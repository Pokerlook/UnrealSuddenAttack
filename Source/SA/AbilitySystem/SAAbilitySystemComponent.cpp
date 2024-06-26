// Fill out your copyright notice in the Description page of Project Settings.


#include "SAAbilitySystemComponent.h"
#include "SA/AbilitySystem/Ability/SAGameplayAbility.h"

USAAbilitySystemComponent::USAAbilitySystemComponent()
{
}

void USAAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const USAGameplayAbility* SAAbility = Cast<USAGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(SAAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}
