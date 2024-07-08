// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SACharacterBase.h"
#include "SAAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class SA_API ASAAICharacter : public ASACharacterBase
{
	GENERATED_BODY()
public:
	ASAAICharacter(const FObjectInitializer& ObjectInitializer);

	//anim
	//virtual FTransform GetWeaponLeftHandSocketTransform() const override;
	//virtual bool IsAiming() const override;
	//anim

protected:
	virtual void BeginPlay() override;
};
