#pragma once
#include <Windows.h>
#include "util.h"
#include "skCrypter.h"
#include "minhook/MinHook.h"
#include "engine.h"
#include "functions.h"
void* (*PEOG)(void*, void*, void*);

namespace Hooks
{
	void* process_event_address;
	void* ProcessEventDetour(UObject* pObject, UObject* pFunction, void* pParams)
	{
		auto func_name = pFunction->GetFullName();
		auto object_name = pObject->GetFullName();

		if (object_name.find("UAC") != std::string::npos) return nullptr;

		if (func_name.find("Tick") != std::string::npos)
		{
			if (GetAsyncKeyState(VK_F1) & 0x1)
			{
				Functions::UpdatePlayerController();
				Functions::UnlockConsole();
			}
		}

		return PEOG(pObject, pFunction, pParams);
	}

	static void Sink()
	{
		MH_CreateHook((void*)process_event_address, ProcessEventDetour, (void**)(&PEOG));
		MH_EnableHook((void*)process_event_address);
	}
}