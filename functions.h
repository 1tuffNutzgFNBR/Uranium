#include <Windows.h>
#include "skCrypter.h"
#include "engine.h"

struct SpawnObjectParams
{
	UObject* ObjectClass;
	UObject* Outer;
	UObject* ReturnValue;
};

namespace Functions
{
	static void UnlockConsole()
	{
		auto FortGameViewportClient = FindObject(crypt("FortGameViewportClient /Engine/Transient.FortEngine_"));
		auto fn = FindObject(crypt("Function /Script/Engine.GameplayStatics.SpawnObject"));
		auto statics = FindObject(crypt("GameplayStatics /Script/Engine.Default__GameplayStatics"));
		auto ConsoleClass = FindObject(crypt("/Script/Engine.Console"));
		auto ViewportConsole = reinterpret_cast<UObject**>((uintptr_t)FortGameViewportClient + 0x40);

		SpawnObjectParams params;
		params.ObjectClass = ConsoleClass;
		params.Outer = FortGameViewportClient;

		ProcessEvent(statics, fn, &params);

		*ViewportConsole = params.ReturnValue;
	}

}