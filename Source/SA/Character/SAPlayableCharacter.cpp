// Fill out your copyright notice in the Description page of Project Settings.


#include "SA/Character/SAPlayableCharacter.h"
#include "SA/Player/SAPlayerState.h"
#include "AbilitySystemComponent.h"

ASAPlayableCharacter::ASAPlayableCharacter()
{

}

void ASAPlayableCharacter::Move(FVector2D Value)
{
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Value.Y);
		AddMovementInput(RightDirection, Value.X);
	}
}

void ASAPlayableCharacter::Look(FVector2D Value)
{
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{		
		PC->AddYawInput(Value.X);
		PC->AddPitchInput(Value.Y);
	}
}

void ASAPlayableCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
}

void ASAPlayableCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void ASAPlayableCharacter::InitAbilityActorInfo()
{
	ASAPlayerState* PS = GetPlayerStateChecked<ASAPlayerState>();
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	AbilitySystemComponent = PS->GetAbilitySystemComponent();
	AttributeSet = PS->GetAttributeSet();
}
