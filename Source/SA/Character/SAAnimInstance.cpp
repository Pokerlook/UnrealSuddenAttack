// Fill out your copyright notice in the Description page of Project Settings.


#include "SAAnimInstance.h"
#include "SA/Interface/AnimInterface.h" 
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

void USAAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AnimIntf = TryGetPawnOwner();
//	Character = Cast<ACharacter>(TryGetPawnOwner());
//	check(AnimIntf);

}

void USAAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!AnimIntf || AnimIntf && !IsValid(AnimIntf->_getUObject()))  //
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

	bUseFABRIK = AnimIntf->ShouldUseIK(); 
	if (bUseFABRIK && EquippedWeaponType!=EWeaponType::None && AnimIntf->GetCharacterMesh())
	{
		LeftHandTransform = AnimIntf->GetWeaponLeftHandSocketTransform();
		//if (!LeftHandTransform.Equals(FTransform::Identity))	// 이거 체크는 AnimBP에서 bool 블렌딩으로 하자
		FVector OutPosition;
		FRotator OutRotation;
		// 무기의 소켓 위치를, 우리 캐릭터의 오른손 기준으로 얻고 싶다.
		AnimIntf->GetCharacterMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));
	}
}
