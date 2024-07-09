#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SATypes.generated.h"


UENUM(BlueprintType)
enum class ECharacterStance : uint8
{
	Stand UMETA(DisplayName = "Stand"),
	Crouch UMETA(DisplayName = "Crouch"),
	Prone UMETA(DisplayName = "Prone")
};

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	TIP_Left UMETA(DisplayName = "Turning Left"),
	TIP_Right UMETA(DisplayName = "Turning Right"),
	TIP_NotTurning UMETA(DisplayName = "Not Turning"),

	TIP_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	MainRangeWeapon UMETA(DisplayName = "MainRangeWeapon"),
	SubRangeWeapon UMETA(DisplayName = "SubRangeWeapon"),
	MeleeWeapon UMETA(DisplayName = "MeleeWeapon"),
	ThrowWeapon UMETA(DisplayName = "ThrowWeapon"),
	SpecialEquipment UMETA(DisplayName = "SpecialEquipment")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	Initial UMETA(DisplayName = "Initial State"),	// 계속 존재
//	InInventory UMETA(DisplayName = "InInventory"),	// 인벤에 있음.  --> 인벤에 들어가면 Destroy
	InUse UMETA(DisplayName = "InUse"),				// 사용중. 또는 장비중.
	Dropped UMETA(DisplayName = "Dropped"),			// 일정 시간 후 사라짐

	DefaultMAX UMETA(DisplayName = "DefaultMAX")
};
