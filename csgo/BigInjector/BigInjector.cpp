#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

void __declspec(noreturn) weMadeAFuckyWucky()
{
    printf("Fucky wucky :'(\n");
    (void)_getwche();
    abort();
}

int main()
{
    const char* path = "C:\\Users\\User\\Downloads\\stream\\meme\\csgo\\Debug\\BigDll.dll";

    HWND hWnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");
    printf("hwnd = %x\n", hWnd);
    DWORD pid;
    GetWindowThreadProcessId(hWnd, &pid);
    printf("CSGO pid = %d\n", pid);

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (!hProcess || hProcess == INVALID_HANDLE_VALUE)
    {
        weMadeAFuckyWucky();
    }

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

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, buf, 0, NULL);

    if (!hThread || hThread == INVALID_HANDLE_VALUE)
    {
        weMadeAFuckyWucky();
    }

    printf("Everything seems okay?");

    //(void)_getwche();
    return 0;
}
