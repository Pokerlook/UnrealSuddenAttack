// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAItemBase.h"
#include "SA/SATypes.h"
#include "SAWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class SA_API ASAWeaponBase : public ASAItemBase
{
	GENERATED_BODY()

public:
	ASAWeaponBase();

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		class USphereComponent* AreaSphere;

	UPROPERTY(VisibleAnywhere)
		EWeaponState WeaponState = EWeaponState::Initial;

};
