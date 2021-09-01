#pragma once

// user mode manual mapper credit to rompse
// this some good paste

#include <windows.h>

#include <functional>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

namespace pe_mapper
{
	class mapping
	{
	public:

		inline mapping(
			std::function<bool(uintptr_t, uintptr_t, size_t)>  write_memory,
			std::function<uintptr_t(size_t)>                   allocate_memory,
			std::function<void(uintptr_t)>                     free_memory,
			std::function<uintptr_t(std::string, std::string)> get_function
		) :
			write_memory(write_memory),
			allocate_memory(allocate_memory),
			free_memory(free_memory),
			get_function(get_function)
		{

		}

		// reads an image into a buffer within this image and
		// sets up required structures for mapping
		inline auto read_image(const std::string_view image_path, const char* entrypoint_export_name) -> bool
		{
			if ( !std::filesystem::exists( image_path ) )
				return false;
			
			std::ifstream file_stream( image_path, std::ios::binary );
			if ( file_stream.fail( ) )
				return false;

			this->raw_image.clear( );
			this->raw_image.assign( ( std::istreambuf_iterator<char>( file_stream ) ), std::istreambuf_iterator<char>( ) );
			file_stream.close( );
			
			this->local_image_base = reinterpret_cast<uintptr_t>(this->raw_image.data());
			this->image_dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(this->local_image_base);
			this->image_nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(this->local_image_base + this->image_dos_header->e_lfanew);
			this->image_size = image_nt_headers->OptionalHeader.SizeOfImage;

			const auto module_to_load = LoadLibraryExA(std::string(image_path).c_str(), NULL, DONT_RESOLVE_DLL_REFERENCES);
			if (!module_to_load)
				return false;
			const auto entrypoint_ptr = (uintptr_t)GetProcAddress(module_to_load, entrypoint_export_name);
			if (!entrypoint_ptr)
			{
				printf("module doesn't export %s\n", entrypoint_export_name);
				return false;
			}
			this->entrypoint_rva = entrypoint_ptr - (uintptr_t)module_to_load;
			printf("manual map entrypoint rva = %p\n", (void*)entrypoint_rva);
			FreeLibrary(module_to_load);

			return true;
		}

		// maps image into whatever context specified by the 
		// read_memory, write_memory, allocate_memory and 
		// free_memory callbacks
		inline auto load_image() -> bool
		{
			if (this->raw_image.empty()) // check that we have previously read the image
				return false;

			// allocate memory for the image
			this->remote_image_base = allocate_memory(this->image_size);
			if (!this->remote_image_base)
				return false;

			printf( "remote_image_base : %p\n", (void*) this->remote_image_base );
			printf( "remote_image_size : %p\n", (void*) this->image_size );
			
			do
			{
				// resolve image imports
				if (!this->resolve_imports())
				{
					puts("resolve_imports failed");
					break;
				}

				printf( "resolved_imports\n" );

				// resolve relocations
				if (!this->resolve_relocations())
				{
					puts("resolved_relocations failed");
					break;
				}

				printf( "resolved_relocations\n" );

				
				// now write the image into the remote process

				// write the dos and nt header
				if (!write_memory(remote_image_base, local_image_base, this->image_nt_headers->OptionalHeader.SizeOfHeaders))
				{
					puts("failed to map pe header");
					break;
				}

				bool mapped_sections = true;
				auto section = IMAGE_FIRST_SECTION(this->image_nt_headers);
				for (auto idx = 0u; idx < this->image_nt_headers->FileHeader.NumberOfSections; idx++, section++)
				{
					const auto section_name = section->Name;
					const auto section_size = min(section->SizeOfRawData, section->Misc.VirtualSize);

					if ( !section_size )
						continue;
					
					const auto mapped_section = remote_image_base + section->VirtualAddress;
					const auto local_section = local_image_base + section->PointerToRawData;

					if (!write_memory(mapped_section, local_section, section_size))
					{
						mapped_sections = false;
						break;
					}
					printf( "mapped section : %i : %s\n", idx, std::string( reinterpret_cast<const char*>( section->Name ) ).c_str() );
				}

				printf( "mapped_sections : %x\n", mapped_sections );

				// if we failed, oof
				if (!mapped_sections)
					break;
				
				printf( "mapped_sections\n" );

				//this->remote_entrypoint = this->remote_image_base + this->image_nt_headers->OptionalHeader.AddressOfEntryPoint;
				this->remote_entrypoint = this->remote_image_base + this->entrypoint_rva;

				printf( "remote_entrypoint : %p\n", (void*)this->remote_entrypoint );
				printf( "remote_image_base : %p\n", (void*)this->remote_image_base );
				//printf( "AddressOfEntryPoint : 0x%lu\n",this->image_nt_headers->OptionalHeader.AddressOfEntryPoint );

#if 0
				#undef max
				std::random_device rd;
				std::mt19937 mt( rd( ) );
				std::uniform_int_distribution<int> dist( 0, std::numeric_limits<uint8_t>::max( ) );
				for ( auto idx = 0u; idx < this->image_nt_headers->OptionalHeader.SizeOfHeaders; idx++ )
					driver->write<uint8_t>( this->remote_image_base, static_cast< uint8_t >( dist( mt ) ) );

				printf( "randomized headers\n" );
#endif

				return true;

			} while (false);

			// error handling: free the remote memory
			free_memory(this->remote_image_base);
			return false;
		}

		//

		inline auto remote_info() const -> std::pair<uintptr_t, uintptr_t>
		{
			return { this->remote_image_base, this->image_size };
		}
		inline auto entrypoint() const -> uintptr_t
		{
			return this->remote_entrypoint;
		}
		inline auto exported(const std::string_view dll_name, const std::string_view name) -> uintptr_t
		{
			const auto local_module = LoadLibraryExA(dll_name.data(), nullptr, DONT_RESOLVE_DLL_REFERENCES);
			printf("local_module : %p\n", local_module);

			if (!local_module)
				return 0;

			const auto local_proc = reinterpret_cast<uintptr_t>(GetProcAddress(local_module, name.data()));
			printf("local_proc : %p\n", (void*)local_proc);

			if (!local_proc)
				return 0;

			return (local_proc - reinterpret_cast<uintptr_t>(local_module)) + this->remote_image_base;
		}

	private:

		__forceinline auto resolve_imports() const -> bool
		{
			const auto& import_dir = this->image_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
			if (import_dir.VirtualAddress && import_dir.Size) // only if we have any imports to resolve
			{
				// go through all modules that we need to import something from
				for (auto import_desc = this->translate<PIMAGE_IMPORT_DESCRIPTOR>(import_dir.VirtualAddress); import_desc->FirstThunk; import_desc++)
				{
					const auto module_name = std::string{ this->translate<const char*>(import_desc->Name) };

					if (module_name.empty())
					{
						printf("empty module name for import thunk at %p\n", import_desc);
						return false;
					}

					// go through all of the function imports for this module
					for (auto thunk_data = this->translate<PIMAGE_THUNK_DATA>(import_desc->FirstThunk); thunk_data->u1.AddressOfData; thunk_data++)
					{
						const auto thunk_import = this->translate<PIMAGE_IMPORT_BY_NAME>(thunk_data->u1.AddressOfData);
						if (!thunk_import->Name)
						{
							printf("bad import by name at 0x%08x\n", thunk_data->u1.AddressOfData);
							return false;
						}

						auto func_name = std::string{ thunk_import->Name };
						const auto function_address = get_function(module_name, func_name);
						if (!function_address) // function was not found
						{
							printf("couldn't find imported function %s!%s\n", module_name.c_str(), func_name.c_str());
							return false;
						}

						// link import
						thunk_data->u1.Function = function_address;
					}
				}
			}

			return true;
		}

		bool process_relocation(uintptr_t image_base_delta, uint16_t data, uint8_t* relocation_base)
		{
#define IMR_RELOFFSET(x)			(x & 0xFFF)

			switch (data >> 12 & 0xF)
			{
			case IMAGE_REL_BASED_HIGH:
			{
				const auto raw_address = reinterpret_cast<int16_t*>(relocation_base + IMR_RELOFFSET(data));
				*raw_address += static_cast<unsigned long>(HIWORD(image_base_delta));
				break;
			}
			case IMAGE_REL_BASED_LOW:
			{
				const auto raw_address = reinterpret_cast<int16_t*>(relocation_base + IMR_RELOFFSET(data));
				*raw_address += static_cast<unsigned long>(LOWORD(image_base_delta));
				break;
			}
			case IMAGE_REL_BASED_HIGHLOW:
			{
				const auto raw_address = reinterpret_cast<size_t*>(relocation_base + IMR_RELOFFSET(data));
				*raw_address += static_cast<size_t>(image_base_delta);
				break;
			}
			case IMAGE_REL_BASED_DIR64:
			{
				auto UNALIGNED raw_address = reinterpret_cast<DWORD_PTR UNALIGNED*>(relocation_base + IMR_RELOFFSET(data));
				*raw_address += image_base_delta;
				break;
			}
			case IMAGE_REL_BASED_ABSOLUTE: // No action required
			case IMAGE_REL_BASED_HIGHADJ: // no action required
			{
				break;
			}
			default:
			{
				throw std::runtime_error("gay relocation!"); // :flushed: ??
				return false;
			}

			}
#undef IMR_RELOFFSET

			return true;
		}

		__forceinline auto resolve_relocations() -> bool
		{
			auto image_base_delta = static_cast<uintptr_t>(static_cast<uintptr_t>(this->remote_image_base) - (this->image_nt_headers->OptionalHeader.ImageBase));

			if (this->image_nt_headers->FileHeader.Characteristics & IMAGE_FILE_RELOCS_STRIPPED)
				return true;

			const auto& reloc_dir = this->image_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
			if (reloc_dir.VirtualAddress && reloc_dir.Size) // only if we have any relocations to resolve
			{
				auto base_relocation = this->translate<PIMAGE_BASE_RELOCATION>(reloc_dir.VirtualAddress);
				void* relocation_end = reinterpret_cast<uint8_t*>(base_relocation) + reloc_dir.Size;

				while (base_relocation < relocation_end)
				{
					auto relocation_base = this->translate< uint8_t* >(base_relocation->VirtualAddress);
					auto num_relocs = (base_relocation->SizeOfBlock - 8) >> 1;
					auto relocation_data = reinterpret_cast<PWORD>(base_relocation + 1);

					for (unsigned long i = 0; i < num_relocs; ++i, ++relocation_data)
					{
						if (this->process_relocation(image_base_delta, *relocation_data, (uint8_t*)relocation_base) == FALSE)
						{
							printf("failed to relocate!");
							return false;
						}
					}

					base_relocation = reinterpret_cast<PIMAGE_BASE_RELOCATION>(relocation_data);
				}
			}

			return true;
		}

		template <typename T> __forceinline auto translate(uintptr_t relative_address) const -> T
		{
			const auto section_count = this->image_nt_headers->FileHeader.NumberOfSections;

			auto section = IMAGE_FIRST_SECTION(this->image_nt_headers);
			for (auto idx = 0u; idx < section_count; idx++, section++)
				if (relative_address >= section->VirtualAddress && relative_address < section->VirtualAddress + section->Misc.VirtualSize)
					return (T)(this->local_image_base + section->PointerToRawData + (relative_address - section->VirtualAddress));

			return T{};
		}

		// callbacks
		std::function<bool(uintptr_t, uintptr_t, size_t)>      write_memory = {};
		std::function<uintptr_t(size_t)>                       allocate_memory = {};
		std::function<void(uintptr_t)>                         free_memory = {};
		std::function<uintptr_t(std::string, std::string)>     get_function = {};

		// image information
		PIMAGE_DOS_HEADER image_dos_header = nullptr;
		PIMAGE_NT_HEADERS image_nt_headers = nullptr;
		uintptr_t          image_size = 0;

		uintptr_t          local_image_base = 0;
		uintptr_t          remote_image_base = 0;
		uintptr_t          entrypoint_rva = 0;
		uintptr_t          remote_entrypoint = 0;

		// raw image
		std::vector<uint8_t> raw_image = {};
	};
}
