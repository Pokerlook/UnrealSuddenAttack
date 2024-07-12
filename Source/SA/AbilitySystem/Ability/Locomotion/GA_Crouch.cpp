// Fill out your copyright notice in the Description page of Project Settings.


#include "SA/AbilitySystem/Ability/Locomotion/GA_Crouch.h"
#include "SA/Interface/AnimInterface.h"
#include "SA/SATagSingleton.h"

#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"

UGA_Crouch::UGA_Crouch()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGA_Crouch::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	IAnimInterface* AnimIntf = Cast< IAnimInterface>(ActorInfo->AvatarActor.Get());
	if (AnimIntf && AnimIntf->IsAnyMontagePlaying())
	{
//		if (ActorInfo->PlayerController.IsValid() && ActorInfo->PlayerController.Get()->GetHUD())
//		{
//			// send message to playercontroller....
//		}

		return false;
	}

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	if (Character->bIsCrouched) return true;
	return Character->CanCrouch();
}

void UGA_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo)) return;

	FGameplayTagContainer OwnerTags;
	UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	AbilitySystemComponent->GetOwnedGameplayTags(OwnerTags);

	const FSAGameplayTags& SATags = FSAGameplayTags::Get();

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	if (OwnerTags.HasTag(SATags.State_Stance_Crouch))
	{
		CrouchToStand(Character);
	}
	else if (OwnerTags.HasTag(SATags.State_Stance_Prone))
	{
		ProneToCrouch(Character);
	}
	else
	{
		StandToCrouch(Character);
	}

}
