#include <Windows.h>
#include <iostream>
#include "minhook/MinHook.h"
#include "util.h"
#include "skCrypter.h"
#include "enginehooks.h"
#include "globals.h"

DWORD WINAPI MainThread(LPVOID)
{
    Util::InitConsole();
    MH_Initialize();
    Util::Log(0, crypt("MinHook initialized!"));
    EngineHooks::Sink();
    Util::Log(0, crypt("EngineHooks sank!"));
    auto pGObjects = Util::FindPattern(crypt("48 8B 05 ? ? ? ? 48 8B 0C C8 48 8B 04 D1"), true, 3);
    CHECKSIG(pGObjects, "Failed to find GObjects address!");
    GObjects = decltype(GObjects)(pGObjects);

    //19
    auto pFNameToString = Util::FindPattern(crypt("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 48 8B F2 4C 8B F1 E8 ? ? ? ? 45 8B 06 33 ED"));
    if (!pFNameToString) {
        //17 - 18
        pFNameToString = Util::FindPattern(crypt("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 33 F6 48 8B F2 44 39 71 04 0F 85 ? ? ? ? 8B 19 0F B7 FB E8 ? ? ? ? 8B CB 48 8D 54 24"));
    }
    CHECKSIG(pFNameToString, "Failed to find FNameToString address!");
    FNameToString = decltype(FNameToString)(pFNameToString);

    // 17 - 19
    auto pFreeMemory = Util::FindPattern(crypt("48 85 C9 0F 84 ? ? ? ? 53 48 83 EC 20 48 89 7C 24 30 48 8B D9 48 8B 3D ? ? ? ? 48 85 FF 0F 84 ? ? ? ? 48 8B 07 4C 8B 40 30 48 8D 05 ? ? ? ? 4C 3B C0"));
    CHECKSIG(pFreeMemory, "Failed to find FreeMemory address!");
    FreeMemory = decltype(FreeMemory)(pFreeMemory);

    // 19
    auto pWorld = Util::FindPattern(crypt("48 8B 05 ? ? ? ? 4D 8B C1"), true, 3);
    if (!pWorld) {
        // 17 - 18
        pWorld = Util::FindPattern(crypt("48 8B 05 ? ? ? ? 4D 8B C2"), true, 3);
    }
    CHECKSIG(pWorld, "Failed to find UWorld address!");
    Globals::GWorld = reinterpret_cast<UObject**>(pWorld);


    Globals::Init();
    Globals::Offsets::Init();
    Util::Log(0, crypt("Functions & Offsets initialized!"));
    
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
