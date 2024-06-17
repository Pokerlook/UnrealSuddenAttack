// Fill out your copyright notice in the Description page of Project Settings.


#include "SAAICharacter.h"
#include "SA/AbilitySystem/SAAbilitySystemComponent.h"
#include "SA/AbilitySystem/SAAttributeSet.h"

ASAAICharacter::ASAAICharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<USAAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<USAAttributeSet>("AttributeSet");
}
