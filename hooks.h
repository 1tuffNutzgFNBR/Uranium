#pragma once
#include <Windows.h>
#include "util.h"
#include "skCrypter.h"
#include "minhook/MinHook.h"
#include "engine.h"
void* (*PEOG)(void*, void*, void*);

namespace Hooks
{
	void* process_event_address;
	void* ProcessEventDetour(UObject* pObject, UObject* pFunction, void* pParams)
	{
		auto func_name = pFunction->GetFullName();
		auto object_name = pObject->GetFullName();

		return PEOG(pObject, pFunction, pParams);
	}
	static void Sink()
	{
		MH_CreateHook((void*)process_event_address, ProcessEventDetour, (void**)(&PEOG));
		MH_EnableHook((void*)process_event_address);
	}
}