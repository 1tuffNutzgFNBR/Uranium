#include <Windows.h>
#include <iostream>
#include "minhook/MinHook.h"
#include "util.h"

DWORD WINAPI MainThread(LPVOID)
{
    Util::InitConsole();
    MH_Initialize();
    Util::Log(0, "MinHook initialized!");
    /*
    MH_CreateHook((void*)PEAddr, ProcessEventDetour, (void**)(&PEOG));
    MH_EnableHook((void*)PEAddr);
    */

    return NULL;
}

BOOL APIENTRY DllMain(HMODULE mod, DWORD reason, LPVOID res)
{
    if (reason == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, MainThread, mod, 0, 0);

    return TRUE;
}
