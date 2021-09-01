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

struct SteamPatch
{
    LPVOID location;
    BYTE patchedCode[5];
    BYTE originalCode[5];
    DWORD originalProtect;
};

HANDLE hProcess;

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};
std::unordered_map<std::pair<std::string, std::string>, SteamPatch, pair_hash> patches;

void UndoSteamPatch(const char* moduleName, const char* procName)
{
    SteamPatch myPatch;

    myPatch.location = GetProcAddress(LoadLibraryA(moduleName), procName);
    if (!myPatch.location)
    {
        printf("No %s!%s??", moduleName, procName);
        weMadeAFuckyWucky();
    }

    memcpy(myPatch.originalCode, myPatch.location, 5);
    ReadProcessMemory(hProcess, myPatch.location, myPatch.patchedCode, 5, NULL);
    VirtualProtectEx(hProcess, myPatch.location, 5, PAGE_EXECUTE_READWRITE, &myPatch.originalProtect);
    WriteProcessMemory(hProcess, myPatch.location, myPatch.originalCode, 5, NULL);
    FlushInstructionCache(hProcess, myPatch.location, 5);

    patches[{moduleName, procName}] = myPatch;
}

void RedoSteamPatch(const char* moduleName, const char* procName)
{
    if (patches.find({ moduleName, procName }) == patches.end())
    {
        puts("ive never seen this man in my life!");
        weMadeAFuckyWucky();
    }

    SteamPatch myPatch = patches[{moduleName, procName}];

    // restore csgo signing patch
    WriteProcessMemory(hProcess, myPatch.location, myPatch.patchedCode, 5, NULL);
    DWORD trash;
    VirtualProtectEx(hProcess, myPatch.location, 5, myPatch.originalProtect, &trash);
    FlushInstructionCache(hProcess, myPatch.location, 5);
}


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
            if (strstr(szModName, szModuleName))
            {
                result = hMods[i];
                break;
            }
        }
    }
    free(hMods);
    return result;
}

int main()
{
    //const char* path = "C:\\Users\\User\\Desktop\\BigDll.dll";
    const char* path = "C:\\Users\\User\\Downloads\\stream\\meme\\csgo\\Debug\\BigDll.dll";

    HWND hWnd = FindWindowA("Valve001", "Counter-Strike: Global Offensive");
    printf("hwnd = %x\n", hWnd);
    DWORD pid;
    GetWindowThreadProcessId(hWnd, &pid);
    printf("CSGO pid = %d\n", pid);

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (!hProcess || hProcess == INVALID_HANDLE_VALUE)
    {
        weMadeAFuckyWucky();
    }

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

    _getwche();

    WriteProcessMemory(hProcess, entrypoint_ptr, dllmainOrig, sizeof(dllmainStub), NULL);
    DWORD trash;
    VirtualProtectEx(hProcess, entrypoint_ptr, sizeof(dllmainStub), oldProtect, &trash);

    return 0;

    // lmao bypass signing requirements
    UndoSteamPatch("ntdll", "LdrLoadDll");
    UndoSteamPatch("ntdll", "NtOpenFile");
    UndoSteamPatch("kernel32", "LoadLibraryExW");
    UndoSteamPatch("kernel32", "LoadLibraryA");
    UndoSteamPatch("kernel32", "LoadLibraryW");
    UndoSteamPatch("kernel32", "LoadLibraryExA");
    UndoSteamPatch("kernelbase", "LoadLibraryExA");
    UndoSteamPatch("kernelbase", "LoadLibraryExW");
    UndoSteamPatch("kernel32", "FreeLibrary");
    UndoSteamPatch("kernelbase", "FreeLibrary");

    LPVOID buf = VirtualAllocEx(hProcess, NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (!buf)
    {
        puts("VirtualAllocEx fails");
        weMadeAFuckyWucky();
    }

    printf("Allocated page = %p\n", buf);

    DWORD nWritten;
    BOOL succ = WriteProcessMemory(hProcess, buf, path, strlen(path), &nWritten);

    if (!succ || nWritten != strlen(path))
    {
        puts("NOT SUCC :((((");
        weMadeAFuckyWucky();
    }

    Sleep(1000);

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, buf, 0, NULL);

    if (!hThread || hThread == INVALID_HANDLE_VALUE)
    {
        weMadeAFuckyWucky();
    }

    printf("Everything seems okay?");

    (void)_getwche();

    // free allocated thing
    VirtualFreeEx(hProcess, buf, 4096, MEM_RELEASE);

    RedoSteamPatch("ntdll", "LdrLoadDll");
    UndoSteamPatch("ntdll", "NtOpenFile");
    RedoSteamPatch("kernel32", "LoadLibraryExW");
    RedoSteamPatch("kernel32", "LoadLibraryA");
    RedoSteamPatch("kernel32", "LoadLibraryW");
    RedoSteamPatch("kernel32", "LoadLibraryExA");
    RedoSteamPatch("kernelbase", "LoadLibraryExA");
    RedoSteamPatch("kernelbase", "LoadLibraryExW");

    (void)_getwche();
    return 0;
}
