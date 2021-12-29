#pragma once

#include "structs.h"
#include "enums.h"

inline void* (*ProcessEvent)(UObject* pObject, UObject* pFunction, void* pParams);
inline UObject* (*SpawnActorLong)(UObject* UWorld, UObject* Class, FTransform const* UserTransformPtr, const FActorSpawnParameters& SpawnParameters);

static UObject* FindObject(std::string name)
{
	for (int32_t i = 0; i < GObjects->NumElements; i++)
	{
		auto object = GObjects->GetByIndex(i);

		if (object == nullptr)
			continue;

		if (object->GetFullName().find(name) != std::string::npos) {
			//std::cout << "Found Object: " << object->GetFullName() << std::endl;
			return object;
		}
	}

	return nullptr;
}

static UObject* FindObjectByShortName(std::string name)
{
	for (int32_t i = 0; i < GObjects->NumElements; i++)
	{
		auto object = GObjects->GetByIndex(i);

		if (object == nullptr)
			continue;

		if (object->GetName() == name)
			return object;
	}

	return nullptr;
}

static DWORD FindOffset(std::string ClassName, std::string VarName)
{
	auto ObjectClass = (UClass*)FindObjectByShortName(ClassName);

	if (ObjectClass != nullptr)
	{
		//std::cout << "[Offset Finder]: Found Class: " << ObjectClass->GetFullName() << std::endl;

		FField* next = ObjectClass->ChildProperties->Next;

		if (next == nullptr)
			return 0;

		auto fPropertyName = ObjectClass->ChildProperties->GetName();

		if (fPropertyName == VarName)
		{
			//std::cout << "[Offset Finder]: Found Offset: " << *(DWORD*)(__int64(ObjectClass->ChildProperties) + 0x4C) << " At: " << VarName << std::endl;
			return *(DWORD*)(__int64(ObjectClass->ChildProperties) + 0x4C);
		}

		while (next)
		{
			auto nextName = next->GetName();

			if (nextName == VarName) {
				//std::cout << "[Offset Finder]: Found Offset: " << *(DWORD*)(__int64(next) + 0x4C) << " At: " << VarName << std::endl;
				return *(DWORD*)(__int64(next) + 0x4C);
			}
			else {
				next = next->Next;
			}
		}
	}

	return 0;
}