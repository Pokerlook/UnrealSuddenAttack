// Fill out your copyright notice in the Description page of Project Settings.


#include "SACharacterMovementComponent.h"
#include "SA/Character/SAPlayableCharacter.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

USACharacterMovementComponent::USACharacterMovementComponent()
{
	NavAgentProps.bCanCrouch = true;
}

USACharacterMovementComponent::FSavedMove_SA::FSavedMove_SA()
{

}

bool USACharacterMovementComponent::FSavedMove_SA::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	// 현재 동작과 새 동작 확인, 결합할 수 있는지. 가능하면 두 움직임이 하나의 패킷으로 결합되어 서버로 전송, 대역폭 절약.
	FSavedMove_SA* NewSAMove = static_cast<FSavedMove_SA*>(NewMove.Get());
	
	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void USACharacterMovementComponent::FSavedMove_SA::Clear()
{
	Super::Clear();

}

uint8 USACharacterMovementComponent::FSavedMove_SA::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();


	return Result;
}

void USACharacterMovementComponent::FSavedMove_SA::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	// 현재 캐릭터의 상태(위치, 속도, 방향, 입력 상태 등)를 기반으로 움직임 데이터를 저장
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	USACharacterMovementComponent* CharacterMovement = Cast<USACharacterMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSlide = CharacterMovement->Safe_bWantsToSlide;
}

void USACharacterMovementComponent::FSavedMove_SA::PrepMoveFor(ACharacter* C)
{
	// 저장된 움직임 데이터를 캐릭터의 현재 상태로 복원, 클라이언트와 서버 간의 상태 동기화
	Super::PrepMoveFor(C);

	USACharacterMovementComponent* CharacterMovement = Cast<USACharacterMovementComponent>(C->GetCharacterMovement());

	CharacterMovement->Safe_bWantsToSlide = Saved_bWantsToSlide;
}

USACharacterMovementComponent::FNetworkPredictionData_Client_SA::FNetworkPredictionData_Client_SA(const UCharacterMovementComponent& ClientMovement)
	:Super(ClientMovement)
{
}

FSavedMovePtr USACharacterMovementComponent::FNetworkPredictionData_Client_SA::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_SA());
}

FNetworkPredictionData_Client* USACharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);

	if (ClientPredictionData == nullptr)
	{
		USACharacterMovementComponent* MutableThis = const_cast<USACharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_SA(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

bool USACharacterMovementComponent::IsMovingOnGround() const
{
	return Super::IsMovingOnGround() || IsCustomMovementMode(CMOVE_Slide);
}

bool USACharacterMovementComponent::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();	// 허공에서 crouch 방지. 허공에서 crouch하게 하려면 crouch 다시 만들어야함..
}

bool USACharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode==MOVE_Custom && CustomMovementMode==InCustomMovementMode;
}

void USACharacterMovementComponent::Slide()
{
	Safe_bWantsToSlide = true;
}

void USACharacterMovementComponent::UnSlide()
{
	Safe_bWantsToSlide = false;
}

void USACharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void USACharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
}

void USACharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	if (MovementMode == MOVE_Walking && !bWantsToCrouch && Safe_bWantsToSlide)
	{
		FHitResult PotentialSlideSurface;
		if (Velocity.SizeSquared() > pow(MinSlideSpeed, 2) && GetSlideSurface(PotentialSlideSurface))
		{
			Safe_EnterSlide();
		}
		else
		{
			Safe_bWantsToSlide = false;
		}
	}

	if (IsCustomMovementMode(CMOVE_Slide) && !Safe_bWantsToSlide)	// slide중에 crouch 해제
	{
		Safe_ExitSlide();
		SetMovementMode(MOVE_Walking);
	}
	//crouch 업데이트 되기 전에 slide 업데이트
	
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void USACharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

}

void USACharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		PhysSlide(deltaTime, Iterations);
		break;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
	}
}

void USACharacterMovementComponent::Safe_EnterSlide()
{
	bWantsToCrouch = true;	// 캡슐 줄일거임

	Velocity += Velocity.GetSafeNormal2D() * SlideEnterImpulse;
	SetMovementMode(MOVE_Custom, CMOVE_Slide);

	UE_LOG(LogTemp, Warning, TEXT("Slide Start"));
}

void USACharacterMovementComponent::Safe_ExitSlide()
{
	bWantsToCrouch = false;


	Safe_bWantsToSlide = false;

	FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(), FVector::UpVector).ToQuat();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, true, Hit);	//3줄에서 회전 교정
	SetMovementMode(MOVE_Walking);

	UE_LOG(LogTemp, Warning, TEXT("Slide End"));
}

void USACharacterMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}
	RestorePreAdditiveRootMotionVelocity();	
	// 위 줄들은 상용구. 어지간한 PhysCustum은 다 쓰는듯
	// slide는 루트모션 안 쓸거라 RestorePreAdditiveRootMotionVelocity 관련해서 루트모션 함수 안 써도 되기는 하는데 그냥 둠

	FHitResult SurfaceHit;
	if (!GetSlideSurface(SurfaceHit) || Velocity.SizeSquared() < pow(MinSlideSpeed, 2))
	{
		Safe_ExitSlide();
		StartNewPhysics(deltaTime, Iterations);	// slide 끝나기 때문에 다른 Phys를 사용한다
		return;
	}

	// Surface Gravity
	Velocity += SlideGravityForce * FVector::DownVector * deltaTime; // v += a * dt

	// Strafe
	if (FMath::Abs(FVector::DotProduct(Acceleration.GetSafeNormal(), UpdatedComponent->GetRightVector())) > .5)
	{
		Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector());	// 왼쪽 오른쪽 가속만 허용
	}
	else
	{
		Acceleration = FVector::ZeroVector;
	}

	// Calc Velocity
	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity()) // 루트 모션 애니메이션을 실행하지 않고 있으며 현재 루트 모션에 의해 속도가 재정의되지 않을 때
	{
		CalcVelocity(deltaTime, Slide_Friction, true, GetMaxBrakingDeceleration()); // bFluid가 true여야 마찰이 사용됨
	}
	ApplyRootMotionToVelocity(deltaTime);

	// Perform Move
	Iterations++;	// 프레임 움직임 수행할때마다 추적하는 변수
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	FQuat OldRotation = UpdatedComponent->GetComponentRotation().Quaternion();
	FHitResult Hit(1.f);
	FVector Adjusted = Velocity * deltaTime;
	FVector VelPlaneDir = FVector::VectorPlaneProject(Velocity, SurfaceHit.Normal).GetSafeNormal();
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(VelPlaneDir, SurfaceHit.Normal).ToQuat();
	SafeMoveUpdatedComponent(Adjusted, NewRotation, true, Hit);	// 여기서 캐릭터가 이동됨, 여기서 sweep을 true로 해야 캐릭터가 벽에 부딪힘

	if (Hit.Time < 1.f)
	{
		HandleImpact(Hit, deltaTime, Adjusted);
		SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);	// 표면을 따라 움직임, 벽에 부딪혀도 벽을 따라 미끄러짐
	}

	FHitResult NewSurfaceHit;
	if (!GetSlideSurface(NewSurfaceHit) || Velocity.SizeSquared() < pow(MinSlideSpeed, 2))
	{
		Safe_ExitSlide();
	}

	// Update Outgoing Velocity & Acceleration
	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;	
	}
}

bool USACharacterMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f * FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
	return GetWorld()->LineTraceSingleByProfile(Hit, Start, End, ProfileName, SACharacterOwner->GetIgnoreCharacterParams());
}

bool USACharacterMovementComponent::CanSlide() const
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5f * FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
	bool bValidSurface = GetWorld()->LineTraceTestByProfile(Start, End, ProfileName, SACharacterOwner->GetIgnoreCharacterParams());
	bool bEnoughSpeed = Velocity.SizeSquared() > pow(MinSlideSpeed, 2);
	return bValidSurface && bEnoughSpeed;
}
