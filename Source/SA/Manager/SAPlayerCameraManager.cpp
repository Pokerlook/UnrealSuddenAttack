// Fill out your copyright notice in the Description page of Project Settings.


#include "SA/Manager/SAPlayerCameraManager.h"
#include "SA/Character/SAPlayableCharacter.h"
#include "SA/Component/SACharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"

ASAPlayerCameraManager::ASAPlayerCameraManager()
{
}

void ASAPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (ASAPlayableCharacter* SACharacter = Cast<ASAPlayableCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		USACharacterMovementComponent* SMC = SACharacter->GetSACharacterMovement();
		FVector TargetCrouchOffset = FVector(0, 0, 
			SMC->GetCrouchedHalfHeight() 
			- SACharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if (SMC->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		if (SMC->IsMovingOnGround())
		{
			OutVT.POV.Location += Offset;
		}
	}
}
