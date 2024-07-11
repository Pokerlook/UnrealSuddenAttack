// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SA/AbilitySystem/Ability/SAGameplayAbility.h"
#include "GA_Crouch.generated.h"

/**
 * 
 */
UCLASS()
class SA_API UGA_Crouch : public USAGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Crouch();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	// 상황별로 다른 Montage 실행. play montage and wait event. 이벤트 때 tag remove&add
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom")
		void StandToCrouch(ACharacter* Character);
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom")
		void ProneToCrouch(ACharacter* Character);
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom")
		void CrouchToStand(ACharacter* Character);
};
