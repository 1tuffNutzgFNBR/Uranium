#pragma once

#include "structs.h"
#include "enums.h"
#include "engine.h"
#include "skCrypter.h"

namespace Globals
{
	inline UObject** GWorld;
	inline UObject* GameplayStatics;
	inline UObject* FortGameviewPortClient;
	inline UObject* ConsoleClass;
	inline UObject* CheatManagerClass;
	inline UObject* PlayerController;

	static inline void Init()
	{
		FortGameviewPortClient = FindObject(crypt("FortGameViewportClient /Engine/Transient.FortEngine_"));
		GameplayStatics = FindObject(crypt("GameplayStatics /Script/Engine.Default__GameplayStatics"));
		ConsoleClass = FindObject(crypt("/Script/Engine.Console"));
		CheatManagerClass = FindObject(crypt("/Script/Engine.CheatManager"));
	}

	namespace Offsets
	{
		inline uintptr_t GameInstanceOffset;
		inline uintptr_t LocalPlayersOffset;
		inline uintptr_t PlayerControllerOffset;

		static inline void Init()
		{
			GameInstanceOffset = FindOffset(crypt("World"), crypt("OwningGameInstance"));
			LocalPlayersOffset = FindOffset(crypt("GameInstance"), crypt("LocalPlayers"));
			PlayerControllerOffset = FindOffset(crypt("Player"), crypt("PlayerController"));
		}
	}
}