// Fill out your copyright notice in the Description page of Project Settings.


#include "SAAICharacter.h"
#include "SA/AbilitySystem/SAAbilitySystemComponent.h"
#include "SA/AbilitySystem/SAAttributeSet.h"

ASAAICharacter::ASAAICharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<USAAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	// GE makes Attribute change, for AI, I just want to know alive or not
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<USAAttributeSet>("AttributeSet");
}

void ASAAICharacter::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
