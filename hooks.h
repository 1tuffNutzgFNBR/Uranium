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

		if (func_name.find("BndEvt__PlayButton_K2Node_ComponentBoundEvent_0_CommonButtonClicked__DelegateSignature") != std::string::npos)
		{
			Functions::UpdatePlayerController();
		}

		if (func_name.find("Tick") != std::string::npos)
		{
			if (GetAsyncKeyState(VK_F1) & 0x1)
			{
				Functions::EnableCheatManager();
			}
			if (GetAsyncKeyState(VK_F2) & 0x1)
			{
				Functions::DisableCheatManager();
			}
			if (GetAsyncKeyState(VK_F3) & 0x1)
			{
				Functions::UpdatePlayerController();
			}
			if (GetAsyncKeyState(VK_F4) & 0x1)
			{
				Functions::CustomSkin("F_MED_ASN_Sarah_Head_02_ATH.F_MED_ASN_Sarah_Head_02_ATH", "CP_028_Athena_Body.CP_028_Athena_Body");
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