#pragma once

#include <Windows.h>
#include "skCrypter.h"
#include "engine.h"
#include "globals.h"

inline bool ProcessEvent(UObject* pObject, UObject* pFunction, void* pParams)
{
	auto vtable = *reinterpret_cast<void***>(pObject);
	auto ProcesseventVtable = static_cast<void(*)(void*, void*, void*)>(vtable[Globals::PEIndex]); if (!ProcesseventVtable) return false;
	ProcesseventVtable(pObject, pFunction, pParams);
	return true;
}

namespace Functions
{
	static UObject* UpdatePlayerController()
	{
		auto FortEngine = FindObject(crypt("FortEngine /Engine/Transient.FortEngine"));
		auto GameInstance = *reinterpret_cast<UObject**>((uintptr_t)FortEngine + __int64(FindOffset("GameEngine", "GameInstance")));
		auto LocalPlayers = *reinterpret_cast<TArray<UObject*>*>((uintptr_t)GameInstance + __int64(FindOffset("GameInstance", "LocalPlayers")));
		auto LocalPlayer = LocalPlayers[0];
		auto PlayerController = *reinterpret_cast<UObject**>((uintptr_t)LocalPlayer + __int64(FindOffset("Player", "PlayerController")));
		Globals::PlayerController = PlayerController;
		return PlayerController;
	}

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

	static void EnableCheatManager()
	{
		auto fn = FindObject(crypt("Function /Script/Engine.GameplayStatics.SpawnObject"));
		auto statics = FindObject(crypt("GameplayStatics /Script/Engine.Default__GameplayStatics"));
		auto CheatManager = reinterpret_cast<UObject**>(__int64(Globals::PlayerController) + __int64(FindOffset("PlayerController", "CheatManager")));
		auto CheatManagerClass = FindObject(crypt("/Script/Engine.CheatManager"));

		SpawnObjectParams params;
		params.ObjectClass = CheatManagerClass;
		params.Outer = Globals::PlayerController;

		ProcessEvent(statics, fn, &params);

		*CheatManager = params.ReturnValue;
	}

	static void DisableCheatManager()
	{
		auto CheatManager = reinterpret_cast<UObject**>((uintptr_t)Globals::PlayerController + __int64(FindOffset("PlayerController", "CheatManager")));
		*CheatManager = nullptr;
	}

	static void CustomSkin(std::string DefaultHeadPart, std::string DefaultBodyPart)
	{
		auto Pawn = *reinterpret_cast<UObject**>((uintptr_t)Globals::PlayerController + __int64(FindOffset("PlayerController", "Pawn")));
		auto PlayerState = *reinterpret_cast<UObject**>((uintptr_t)Pawn + __int64(FindOffset("Pawn", "PlayerState")));
		auto Hero = FindObject(crypt("FortHero /Engine/Transient.FortHero"));
		auto CharacterParts = reinterpret_cast<TArray<UObject*>*>((uintptr_t)Hero + __int64(FindOffset("FortHero", "CharacterParts")));

		auto Head = FindObject(DefaultBodyPart);
		auto Body = FindObject(DefaultBodyPart);

		CharacterParts->operator[](1) = Head;
		CharacterParts->operator[](0) = Body;

		auto KismetLib = FindObject(crypt("FortKismetLibrary /Script/FortniteGame.Default__FortKismetLibrary"));
		auto fn = FindObject(crypt("Function /Script/FortniteGame.FortKismetLibrary.ApplyCharacterCosmetics"));

		struct {
			UObject* WorldContextObject;
			TArray<UObject*> CharacterParts;
			UObject* PlayerState;
			bool bSuccess;
		} params;

		params.WorldContextObject = (*Globals::GWorld);
		params.CharacterParts = *CharacterParts;
		params.PlayerState = PlayerState;

		ProcessEvent(KismetLib, fn, &params);
	}

	static void SwitchLevel(FString URL)
	{
		auto fn = FindObject(crypt(""));
		ProcessEvent(Globals::PlayerController, fn, &URL);
	}
}