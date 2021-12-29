#pragma once
#include <Windows.h>
#include "util.h"
#include "skCrypter.h"
#include "minhook/MinHook.h"

namespace EngineHooks
{
	void RequestExitWithStatusDetour(bool Force, uint8_t ReturnCode)
	{
		printf(skCrypt("Game tried to exit"));
	}

	void* __fastcall NotificationDetour(wchar_t** a1, unsigned __int8 a2, __int64 a3, char a4)
	{
		return NULL;
	}

	static void Sink()
	{
		auto noti_addr = Util::FindPattern(skCrypt("4C 8B DC 55 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 89 5B 20"));
		auto rewt_addr = Util::FindPattern(skCrypt("48 8B C4 48 89 58 18 88 50 10 88 48 08 57 48 83 EC 30"));

		CHECKSIG(noti_addr, skCrypt("Failed to find Notification address. Remember this is a beta build, things like this can happen. If you see this, please contact us in the Discord server."));
		CHECKSIG(rewt_addr, skCrypt("Failed to find RequestExitWithStatus address. Remember this is a beta build, things like this can happen. If you see this, please contact us in the Discord server."));

		MH_CreateHook(reinterpret_cast<void*>(noti_addr), NotificationDetour, nullptr);
		MH_EnableHook(reinterpret_cast<void*>(noti_addr));
		MH_CreateHook(reinterpret_cast<void*>(rewt_addr), RequestExitWithStatusDetour, nullptr);
		MH_EnableHook(reinterpret_cast<void*>(rewt_addr));
	}
}