#include <Windows.h>
#include <winternl.h>
#pragma comment(lib, "Shlwapi.lib")
#include <Shlwapi.h>
#include <stdio.h>

#include "pe_mapper.hpp"

/*
auto remote_execute( const uint64_t address) -> bool
{
    if (driver->add_module(xorstr("user32.dll")) != drv_result::success)
        return false;
        
    const auto user32_mod = LoadLibraryA(xorstr("user32.dll"));
    if (!user32_mod)
        return false;

    uint64_t function = 0;
    if ( attempt_count > 8 )
        attempt_count = 0;

    if ( attempt_count <= 4 )
        function = reinterpret_cast< uint64_t >( GetProcAddress( user32_mod, is_wide ? xorstr( "GetMonitorInfoW" ) : xorstr( "GetMonitorInfoA" ) ) );
    else if ( attempt_count <= 8 && attempt_count > 4 )
        function = reinterpret_cast< uint64_t >( GetProcAddress( user32_mod, is_wide ? xorstr( "PeekMessageW" ) : xorstr( "PeekMessageA" ) ) );
        
    if (!function)
        return false;

    printf( "trying to remote_execute : 0x%llx\n", address );
    printf( "user32_mod : 0x%llx\n", reinterpret_cast<uint64_t>( user32_mod ) );
    printf( "function : 0x%llx\n",  function );

    uint8_t shellcode[] =
    {
        0x48, 0xb8, 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00, 0xFF, 0xE0
    };
        
    printf( "sizeof(shellcode) = 0x%llx\n", sizeof( shellcode ) );
    printf( "sizeof(shellcode)+2 = 0x%llx\n", sizeof( shellcode )+2 );

    std::vector<uint8_t> backup_shell{};
    for (uint8_t idx = 0; idx < sizeof(shellcode) + 2; idx++)
        backup_shell.push_back(driver->read<uint8_t>(function + idx));
        
    *reinterpret_cast<uint64_t*>(&shellcode[2]) = address;

    printf( "setup shellcode\n" );
        
    driver->write_protected(function, shellcode);

    std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
    printf( "wrote shellcode\n" );

    uint8_t idx{};
    for (const auto& byte : backup_shell)
        driver->write_protected<char>(function + idx++, byte);

    printf( "reverted shellcode\n" );

    attempt_count++;

    return true;
}
*/

#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <psapi.h>

#include <unordered_map>
#include <string>
#include <utility>

void __declspec(noreturn) weMadeAFuckyWucky()
{
    printf("Fucky wucky :'(\n");
    (void)_getwche();
    abort();
}

HANDLE hProcess;

// returns an array of handles for all modules loaded in the target process
DWORD get_all_modules(HANDLE child_handle, HMODULE** modules) {
    DWORD module_handle_storage_size = 1024 * sizeof(HMODULE);
    HMODULE* module_handles = (HMODULE*)malloc(module_handle_storage_size);
    DWORD hmodules_size;
    while (true) {
        if (!EnumProcessModulesEx(child_handle, module_handles, module_handle_storage_size, &hmodules_size, LIST_MODULES_ALL)) {
            printf("EnumProcessModules failed, %x\n", GetLastError());
            weMadeAFuckyWucky();
        }
        if (hmodules_size <= module_handle_storage_size) break;
        module_handle_storage_size *= 2;
        module_handles = (HMODULE*)realloc(module_handles, module_handle_storage_size);
    }
    *modules = module_handles;
    //SAYF("Get all modules:%d\n", hmodules_size / sizeof(HMODULE));
    return hmodules_size / sizeof(HMODULE);
}

char* stristr(const char* haystack, const char* needle) {
    do {
        const char* h = haystack;
        const char* n = needle;
        while (tolower((unsigned char)*h) == tolower((unsigned char)*n) && *n) {
            h++;
            n++;
        }
        if (*n == 0) {
            return (char*)haystack;
        }
    } while (*haystack++);
    return 0;
}

HMODULE FindModule(HANDLE hProcess, const char* szModuleName)
{
    HMODULE* hMods;
    size_t nModules = get_all_modules(hProcess, &hMods);
    HMODULE result = NULL;
    for (unsigned int i = 0; i < nModules; i++)
    {
        char szModName[MAX_PATH];
        if (GetModuleFileNameExA(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(char)))
        {
            if (stristr(szModName, szModuleName))
            {
                result = hMods[i];
                break;
            }
        }
    }
    free(hMods);
    return result;
}

void DoWithClientDllDisabled(std::function<NTSTATUS()> what)
{
    // kill client.dll shit
    HMODULE hClient = LoadLibraryExA("H:\\Steam\\steamapps\\common\\Counter-Strike Global Offensive\\csgo\\bin\\client.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
    if (!hClient)
    {
        puts("no hclient");
        weMadeAFuckyWucky();
    }
    PIMAGE_DOS_HEADER image = (PIMAGE_DOS_HEADER)hClient;
    PIMAGE_NT_HEADERS pe = (PIMAGE_NT_HEADERS)((uintptr_t)hClient + image->e_lfanew);
    ULONG entrypoint_offset = pe->OptionalHeader.AddressOfEntryPoint;
    printf("entrypoint offset = %x\n", entrypoint_offset);
    FreeLibrary(hClient);

    //xor eax,eax
    //inc eax
    //ret 0xc
    BYTE dllmainStub[] = { 0x31, 0xc0, 0x40, 0xc2, 0x0c, 0x00 };
    BYTE dllmainOrig[sizeof(dllmainStub)] = { 0 };

    hClient = FindModule(hProcess, "\\client.dll");
    printf("hclient = %p\n", hClient);
    if (!hClient)
    {
        weMadeAFuckyWucky();
    }
    LPVOID entrypoint_ptr = (LPVOID)((uintptr_t)hClient + (uintptr_t)entrypoint_offset);
    printf("entrypoint at %p\n", entrypoint_ptr);

    ReadProcessMemory(hProcess, entrypoint_ptr, dllmainOrig, sizeof(dllmainStub), NULL);
    DWORD oldProtect;
    VirtualProtectEx(hProcess, entrypoint_ptr, sizeof(dllmainStub), PAGE_EXECUTE_READWRITE, &oldProtect);
    WriteProcessMemory(hProcess, entrypoint_ptr, dllmainStub, sizeof(dllmainStub), NULL);

    printf("old code: %02x %02x %02x %02x %02x\n", dllmainOrig[0], dllmainOrig[1], dllmainOrig[2], dllmainOrig[3], dllmainOrig[4]);

    NTSTATUS callback_result = what();
    if (callback_result != 0)
    {
        printf("Wow, callback failed: returned 0x%08x\n", callback_result);
    }

    WriteProcessMemory(hProcess, entrypoint_ptr, dllmainOrig, sizeof(dllmainStub), NULL);
    DWORD trash;
    VirtualProtectEx(hProcess, entrypoint_ptr, sizeof(dllmainStub), oldProtect, &trash);

    puts("restored dllmain of client.dll");

    if (callback_result != 0)
    {
        weMadeAFuckyWucky();
    }
}

NTSTATUS DoManualMap(const char* dll_name)
{
    pe_mapper::mapping dll_mapping(
        // write memory
        [](uintptr_t address, uintptr_t buffer, size_t size) -> bool
        {
            SIZE_T nWritten;
            WriteProcessMemory(hProcess, (LPVOID)address, (LPCVOID)buffer, size, &nWritten);
            return nWritten == size;
        },
        // size_t memory
        [&](size_t size) -> uintptr_t
        {
            return (uintptr_t)VirtualAllocEx(hProcess, NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        },
        // free memory
        [](uintptr_t address) -> void
        {
            VirtualFreeEx(hProcess, (LPVOID) address, 0, MEM_RELEASE);
        },
        // get function
        [](std::string module_name, std::string function_name) -> uintptr_t
        {
            const auto ssss = std::filesystem::path(module_name).filename().string();

            const uintptr_t remote_module_base = (uintptr_t)FindModule(hProcess, ssss.c_str());
            if (!remote_module_base)
            {
                printf("couldnt find module %s in remote process\n", ssss.c_str());
                return 0;
            }

            const auto local_module =
                reinterpret_cast<uintptr_t>(
                    LoadLibraryExA(module_name.c_str(), nullptr, DONT_RESOLVE_DLL_REFERENCES));
            if (!local_module)
            {
                printf("failed to load module %s locally\n", module_name.c_str());
                return 0;
            }

            const auto local_export =
                reinterpret_cast<uintptr_t>(
                    GetProcAddress(reinterpret_cast<HMODULE>(local_module), function_name.c_str()));
            if (!local_export)
            {
                printf("GetProcAddress(%s, %s) failed\n", module_name.c_str(), function_name.c_str());
                return 0;
            }

            return (local_export - local_module) + remote_module_base;
        }
    );

    if (!dll_mapping.read_image(dll_name, "_ManualMapEntrypoint@4"))
        return 1;

    if (!dll_mapping.load_image())
        return 2;
    
    uintptr_t remote_base = dll_mapping.remote_info().first;
    uintptr_t remote_entrypoint = dll_mapping.entrypoint();
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)remote_entrypoint, (LPVOID)remote_base, 0, NULL);
    if (!hThread)
    {
        printf("hThread fails, GLE=%d\n", GetLastError());
        return 3;
    }
    printf("CreateRemoteThread succeeds, hThread=%p\n", hThread);
    CloseHandle(hThread);

    return 0;
}

int main()
{
    char dll_path[MAX_PATH] = { 0 };
    GetModuleFileNameA(GetModuleHandle(NULL), dll_path, sizeof(dll_path));
    PathAppendA(dll_path, "..\\BigDll.dll");
    printf("Will manual map %s\n", dll_path);

    HWND hWnd = FindWindowA("Valve001", "Counter-Strike: Global Offensive");
    //HWND hWnd = FindWindowA(NULL, "Untitled - Notepad");
    if (!hWnd)
    {
        puts("game not running?");
        return 1;
    }

    printf("hwnd = %p\n", hWnd);
    DWORD pid;
    GetWindowThreadProcessId(hWnd, &pid);
    printf("CSGO pid = %d\n", pid);

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    printf("hProcess = %p\n", hProcess);

    if (!hProcess || hProcess == INVALID_HANDLE_VALUE)
    {
        weMadeAFuckyWucky();
    }

    printf("Hello world\n");

    DoWithClientDllDisabled([dll_path]() { return DoManualMap(dll_path); });

    printf("Everything is ok\n");
    return 0;
}
