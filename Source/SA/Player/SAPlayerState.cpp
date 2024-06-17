// Fill out your copyright notice in the Description page of Project Settings.


#include "SA/Player/SAPlayerState.h"
#include "SA/AbilitySystem/SAAbilitySystemComponent.h"
#include "SA/AbilitySystem/SAAttributeSet.h"

ASAPlayerState::ASAPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<USAAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); 
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);	// Player HUD needs Attribute info

	AttributeSet = CreateDefaultSubobject<USAAttributeSet>("AttributeSet");

}

UAbilitySystemComponent* ASAPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
