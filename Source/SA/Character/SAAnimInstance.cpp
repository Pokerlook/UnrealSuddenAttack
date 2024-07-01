// Fill out your copyright notice in the Description page of Project Settings.


#include "SAAnimInstance.h"
#include "SA/Interface/AnimInterface.h" 
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

void USAAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AnimIntf = TryGetPawnOwner();
	Character = Cast<ACharacter>(TryGetPawnOwner());
//	check(AnimIntf);

}

void USAAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!AnimIntf || AnimIntf && !IsValid(AnimIntf->_getUObject()) )  //
	{
//		UE_LOG(LogTemp, Warning, TEXT("Pawn Owner doesn't have AnimInterface"));
		return;
	}

	Speed = AnimIntf->GetSpeed();
	Direction = AnimIntf->GetDirection();

	Yaw = AnimIntf->GetYaw();
	Pitch = AnimIntf->GetPitch();

	bIsInAir = AnimIntf->IsInAir();
	bIsAccelerating = AnimIntf->IsAccelerating();
	bIsAiming = AnimIntf->IsAiming();

	CharacterStance = AnimIntf->GetStance();
	EquippedWeaponType = AnimIntf->GetEquippedWeaponType();

}
