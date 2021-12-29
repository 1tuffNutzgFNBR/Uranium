#include <Windows.h>
#include <iostream>
#include "minhook/MinHook.h"
#include "util.h"
#include "skCrypter.h"
#include "enginehooks.h"

DWORD WINAPI MainThread(LPVOID)
{
    Util::InitConsole();
    MH_Initialize();
    Util::Log(0, skCrypt("MinHook initialized!").decrypt());
    EngineHooks::Sink();
    Util::Log(0, skCrypt("EngineHooks sank!").decrypt());
    
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
