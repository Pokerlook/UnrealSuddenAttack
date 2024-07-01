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
enum class EWeaponState : uint8
{
	Initial UMETA(DisplayName = "Initial State"),
	Equipped UMETA(DisplayName = "Equipped"),
	Dropped UMETA(DisplayName = "Dropped"),

	DefaultMAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(Atomic, BlueprintType)
struct FS_ItemStaticData
{
	GENERATED_USTRUCT_BODY()

public:
	FS_ItemStaticData() : ItemName(), ItemExplanation(), Icon(), RefClass(), StackSize(1)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText ItemExplanation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ASAItemBase> RefClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int StackSize;
};
