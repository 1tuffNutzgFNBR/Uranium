#pragma once
#pragma once

#include <string>
#include <locale>
#include <Windows.h>
#include "Enums.h"

template<class T>
struct TArray
{
	friend class FString;

public:
	inline TArray()
	{
		Data = NULL;
		Count = Max = 0;
	};

	inline INT Num() const
	{
		return Count;
	};

	inline T& operator[](int i)
	{
		return Data[i];
	};

	inline T& operator[](int i) const
	{
		return Data[i];
	};

	inline BOOL IsValidIndex(int i) const
	{
		return i < Num();
	}

	inline void Add(T InputData)
	{
		Data = (T*)realloc(Data, sizeof(T) * (Count + 1));
		Data[Count++] = InputData;
		Max = Count;
	};

	inline void Remove(int32_t Index)
	{
		TArray<T> NewArray;
		for (int i = 0; i < this->Count; i++)
		{
			if (i == Index)
				continue;

			NewArray.Add(this->operator[](i));
		}
		this->Data = (T*)realloc(NewArray.Data, sizeof(T) * (NewArray.Count));
		this->Count = NewArray.Count;
		this->Max = NewArray.Count;
	}

	T* Data;
	INT32 Count;
	INT32 Max;
};

struct FString : private TArray<wchar_t>
{
	FString()
	{
	};

	FString(const wchar_t* other)
	{
		Max = Count = *other ? std::wcslen(other) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	}

	bool IsValid() const
	{
		return Data != nullptr;
	}

	const wchar_t* c_str() const
	{
		return Data;
	}

	std::string ToString() const
	{
		auto length = std::wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
};

template <class TEnum>
class TEnumAsByte
{
public:
	TEnumAsByte()
	{
	}

	TEnumAsByte(TEnum _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit TEnumAsByte(int32_t _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit TEnumAsByte(uint8_t _value)
		: value(_value)
	{
	}

	operator TEnum() const
	{
		return static_cast<TEnum>(value);
	}

	TEnum GetValue() const
	{
		return static_cast<TEnum>(value);
	}

private:
	uint8_t value;
};

struct FName;

void(__fastcall* FNameToString)(FName* _This, FString& OutStr);
void(__fastcall* FreeMemory)(__int64);

struct FName
{
	int32_t ComparisonIndex;
	int32_t DisplayIndex;

	inline std::string GetName()
	{
		FString Buffer;

		FNameToString(this, Buffer);

		std::string RetName = (const char*)Buffer.c_str();

		FreeMemory((__int64)Buffer.c_str());

		return RetName;
	}
};

struct UObject
{
	void** VFT;
	int32_t ObjectFlags;
	int32_t InternalIndex;
	UObject* Class;
	FName Name;
	UObject* Outer;

	bool IsA(UObject* cmp) const
	{
		if (cmp == Class)
			return true;
		return false;
	}

	std::string GetName()
	{
		if (!this)
			return "";

		return Name.GetName();
	}

	std::string GetFullName()
	{
		std::string temp;

		for (auto outer = Outer; outer; outer = outer->Outer)
		{
			temp = outer->GetName() + "." + temp;
		}

		temp = reinterpret_cast<UObject*>(Class)->GetName() + " " + temp + this->GetName();
		return temp;
	}
};

struct FField
{
	void* VFT;
	void* Class;
	void* Owner;
	void* padding;
	FField* Next;
	FName Name;
	EObjectFlags FlagsPrivate;

	std::string GetName()
	{
		return Name.GetName();
	}

	std::string GetTypeName()
	{
		return (*static_cast<FName*>(Class)).GetName();
	}

	std::string GetFullName()
	{
		std::string temp;

		for (auto outer = Next; outer; outer = outer->Next)
		{
			temp = outer->GetName() + "." + temp;
		}

		temp = static_cast<UObject*>(Class)->GetName() + " " + temp + this->GetName();
		return temp;
	}
};

struct FProperty : FField
{
	int32_t ArrayDim;
	int32_t ElementSize;
	EPropertyFlags PropertyFlags;
	uint16_t RepIndex;
	TEnumAsByte<ELifetimeCondition> BlueprintReplicationCondition;
	int32_t Offset_Internal;
	FName RepNotifyFunc;
	FProperty* PropertyLinkNext;
	FProperty* NextRef;
	FProperty* DestructorLinkNext;
	FProperty* PostConstructLinkNext;
};

struct UField : UObject
{
	UField* Next;
	void* padding_01;
	void* padding_02;
};

struct UStruct : UField
{
	UStruct* SuperStruct;
	UField* Children;
	FField* ChildProperties;
	int32_t PropertiesSize;
	int32_t MinAlignment;
	TArray<uint8_t> Script;
	FProperty* PropertyLink;
	FProperty* RefLink;
	FProperty* DestructorLink;
	FProperty* PostConstructLink;
};

struct UClass : UStruct
{
};

struct FGuid
{
	int A;
	int B;
	int C;
	int D;
};

struct FUObjectItem
{
	UObject* Object;
	DWORD Flags;
	DWORD ClusterIndex;
	DWORD SerialNumber;
	DWORD SerialNumber2;
};

struct PreFUObjectItem
{
	FUObjectItem* FUObject[10];
};

struct FUObjectArray
{
	PreFUObjectItem* ObjectArray;
	BYTE unknown1[8];
	int32_t MaxElements;
	int32_t NumElements;

	void NumChunks(int* start, int* end) const
	{
		int cStart = 0, cEnd = 0;

		if (!cEnd)
		{
			while (true)
			{
				if (ObjectArray->FUObject[cStart] == nullptr)
				{
					cStart++;
				}
				else
				{
					break;
				}
			}

			cEnd = cStart;
			while (true)
			{
				if (ObjectArray->FUObject[cEnd] == nullptr)
				{
					break;
				}
				cEnd++;
			}
		}

		*start = cStart;
		*end = cEnd;
	}

	UObject* GetByIndex(int32_t index) const
	{
		int cStart = 0, cEnd = 0;
		int chunkIndex, chunkSize = 0xFFFF, chunkPos;
		FUObjectItem* Object;

		NumChunks(&cStart, &cEnd);

		chunkIndex = index / chunkSize;
		if (chunkSize * chunkIndex != 0 &&
			chunkSize * chunkIndex == index)
		{
			chunkIndex--;
		}

		chunkPos = cStart + chunkIndex;
		if (chunkPos < cEnd)
		{
			Object = ObjectArray->FUObject[chunkPos] + (index - chunkSize * chunkIndex);
			if (!Object) { return nullptr; }

			return Object->Object;
		}

		return nullptr;
	}
};

FUObjectArray* GObjects;

struct FVector
{
	float X;
	float Y;
	float Z;

	FVector()
		: X(0), Y(0), Z(0)
	{
	}

	FVector(float x, float y, float z)
		: X(x),
		Y(y),
		Z(z)
	{
	}

	FVector operator-(FVector v)
	{
		return FVector(X - v.X, Y - v.Y, Z - v.Z);
	}

	FVector operator+(FVector v)
	{
		return FVector(X + v.X, Y + v.Y, Z + v.Z);
	}

	float Distance(FVector v)
	{
		return ((X - v.X) * (X - v.X) +
			(Y - v.Y) * (Y - v.Y) +
			(Z - v.Z) * (Z - v.Z));
	}
};

struct FRotator
{
	float Pitch;
	float Yaw;
	float Roll;

	FRotator()
		: Pitch(0),
		Yaw(0),
		Roll(0)
	{
	}

	FRotator(float pitch, float yaw, float roll)
		: Pitch(pitch),
		Yaw(yaw),
		Roll(roll)
	{
	}
};

struct FQuat
{
	float W, X, Y, Z;

	FQuat()
		: W(0),
		X(0),
		Y(0),
		Z(0)
	{
	}

	FQuat(float w, float x, float y, float z)
		: W(w),
		X(x),
		Y(y),
		Z(z)
	{
	}
};

struct FTransform
{
	FQuat Rotation;
	FVector Translation;
	char UnknownData_1C[0x4];
	FVector Scale3D;
	char UnknownData_2C[0x4];
};

struct RBitField
{
	unsigned char A : 1;
	unsigned char B : 1;
	unsigned char C : 1;
	unsigned char D : 1;
	unsigned char E : 1;
	unsigned char F : 1;
	unsigned char G : 1;
	unsigned char H : 1;
};

struct FFortItemEntry
{
	unsigned char Unk00[0x1A0];
};

struct FFortAthenaLoadout
{
	FString BannerIconId;
	FString BannerColorId;
	UObject* SkyDiveContrail;
	UObject* Glider;
	UObject* Pickaxe;
	bool bIsDefaultCharacter;
	unsigned char UnknownData00[0x7];
	UObject* Character;
	TArray<UObject*> CharacterVariantChannels;
	bool bForceUpdateVariants;
	unsigned char UnknownData01[0x7];
	UObject* Hat;
	UObject* Backpack;
	UObject* LoadingScreen;
	UObject* BattleBus;
	UObject* VehicleDecoration;
	UObject* CallingCard;
	UObject* MapMarker;
	TArray<UObject*> Dances;
	UObject* VictoryPose;
	UObject* MusicPack;
	UObject* ItemWrapOverride;
	TArray<UObject*> ItemWraps;
	UObject* CharmOverride;
	TArray<UObject*> Charms;
	UObject* PetSkin;
};

struct FActorSpawnParameters
{
	unsigned char Unk00[0x40];
};