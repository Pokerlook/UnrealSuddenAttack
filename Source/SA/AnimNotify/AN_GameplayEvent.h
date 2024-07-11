// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AN_GameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class SA_API UAN_GameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:

	UPROPERTY(EditAnywhere)
		FGameplayEventData Payload;
};
