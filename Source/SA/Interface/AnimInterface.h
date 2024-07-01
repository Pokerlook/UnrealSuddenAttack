// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SA/SATypes.h"
#include "AnimInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SA_API IAnimInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual float GetSpeed() const = 0;
	virtual float GetDirection() const = 0;
	virtual float GetYaw() const = 0;
	virtual float GetPitch() const = 0;

	virtual ECharacterStance GetStance() const = 0;
	virtual EWeaponType GetEquippedWeaponType() const = 0;

	virtual bool IsInAir() const = 0;
	virtual bool IsAccelerating() const = 0;
	virtual bool IsAiming() const = 0;

};
