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
	Initial UMETA(DisplayName = "Initial State"),	// 계속 존재
	Equipped UMETA(DisplayName = "Equipped"),
	Dropped UMETA(DisplayName = "Dropped"),		// 일정 시간 후 사라짐

	DefaultMAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(Atomic, BlueprintType)
struct FS_ItemStaticData
{
	GENERATED_USTRUCT_BODY()

public:
	FS_ItemStaticData() : IdentifierTag(), Name(), ItemExplanation(), Icon(), RefClass(), StackSize(1)
	{}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag IdentifierTag;	// 이걸로 데이터베이스에서 서치.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText ItemExplanation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UTexture2D* Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<class ASAItemBase> RefClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int StackSize;
};
