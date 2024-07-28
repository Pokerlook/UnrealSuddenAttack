// Fill out your copyright notice in the Description page of Project Settings.


#include "SACharacterMovementComponent.h"
#include "SA/Character/SAPlayableCharacter.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

USACharacterMovementComponent::USACharacterMovementComponent()
{
	NavAgentProps.bCanCrouch = true; 
	bCanWalkOffLedgesWhenProned = false;
	bWantsToProne = false;
	bProneLocked = false;
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

	bWantsToProne = false;
	bProneLocked = false;
}

uint8 USACharacterMovementComponent::FSavedMove_SA::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bWantsToProne)
	{
		Result |= FLAG_Prone;
	}

	return Result;
}

void USACharacterMovementComponent::FSavedMove_SA::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	// 현재 캐릭터의 상태(위치, 속도, 방향, 입력 상태 등)를 기반으로 움직임 데이터를 저장
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	USACharacterMovementComponent* CharacterMovement = Cast<USACharacterMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSlide = CharacterMovement->Safe_bWantsToSlide;
	bWantsToProne = Cast<ASAPlayableCharacter>(C)->GetSACharacterMovement()->bWantsToProne;
	bProneLocked = Cast<ASAPlayableCharacter>(C)->GetSACharacterMovement()->bProneLocked;
}

void USACharacterMovementComponent::FSavedMove_SA::PrepMoveFor(ACharacter* C)
{
	// 저장된 움직임 데이터를 캐릭터의 현재 상태로 복원, 클라이언트와 서버 간의 상태 동기화
	Super::PrepMoveFor(C);

	USACharacterMovementComponent* CharacterMovement = Cast<USACharacterMovementComponent>(C->GetCharacterMovement());

	CharacterMovement->Safe_bWantsToSlide = Saved_bWantsToSlide;
	Cast<ASAPlayableCharacter>(C)->GetSACharacterMovement()->bProneLocked = bProneLocked;
}

USACharacterMovementComponent::FNetworkPredictionData_Client_SA::FNetworkPredictionData_Client_SA(const UCharacterMovementComponent& ClientMovement)
	:Super(ClientMovement)
{
}

FSavedMovePtr USACharacterMovementComponent::FNetworkPredictionData_Client_SA::AllocateNewMove()
{
//	return MakeShared<FSavedMove_SA>();
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

bool USACharacterMovementComponent::HasValidData() const
{
	return Super::HasValidData() && IsValid(SACharacterOwner);
}

void USACharacterMovementComponent::PostLoad()
{
	Super::PostLoad();

	SACharacterOwner = Cast<ASAPlayableCharacter>(PawnOwner);
}

void USACharacterMovementComponent::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
	Super::SetUpdatedComponent(NewUpdatedComponent);

	SACharacterOwner = Cast<ASAPlayableCharacter>(PawnOwner);
}

float USACharacterMovementComponent::GetMaxAcceleration() const
{
	if (IsProned() && IsMovingOnGround())
	{
		return MaxAccelerationProned;
	}
	return Super::GetMaxAcceleration();
}

bool USACharacterMovementComponent::IsMovingOnGround() const
{
	return Super::IsMovingOnGround() || IsCustomMovementMode(CMOVE_Slide);
}

bool USACharacterMovementComponent::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();	// 허공에서 crouch 방지. 허공에서 crouch하게 하려면 crouch 다시 만들어야함..
}

float USACharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	if (IsProned() && IsMovingOnGround())
	{
		return BrakingDecelerationProned;
	}
	if (MovementMode != MOVE_Custom) return Super::GetMaxBrakingDeceleration();

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		return BrakingDecelerationSliding;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
			return -1.f;
	}
}

void USACharacterMovementComponent::CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration)
{
	if (IsProned() && IsMovingOnGround())
	{
		Friction = GroundFrictionProned;
	}
	Super::CalcVelocity(DeltaTime, Friction, bFluid, BrakingDeceleration);
}

void USACharacterMovementComponent::ApplyVelocityBraking(float DeltaTime, float Friction, float BrakingDeceleration)
{
	if (IsProned() && IsMovingOnGround())
	{
		Friction = (bUseSeparateBrakingFriction ? BrakingFrictionProned : GroundFrictionProned);
	}
	Super::ApplyVelocityBraking(DeltaTime, Friction, BrakingDeceleration);
}

bool USACharacterMovementComponent::CanWalkOffLedges() const
{
	if (!bCanWalkOffLedgesWhenProned && IsProned())
	{
		return false;
	}

	return Super::CanWalkOffLedges();
}

bool USACharacterMovementComponent::CanAttemptJump() const
{
	return Super::CanAttemptJump() && !bWantsToProne;
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

bool USACharacterMovementComponent::IsProneLocked() const
{
	if (CharacterOwner && CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)
	{
		// Sim proxies don't prone lock
		return false;
	}
	return bProneLocked;
}

bool USACharacterMovementComponent::IsProneLockOnTimer() const
{
	return GetRemainingProneLockCooldown() > 0.f;
}

float USACharacterMovementComponent::GetRemainingProneLockCooldown() const
{
	const float CurrentTimestamp = GetTimestamp();
	const float RemainingCooldown = ProneLockDuration - (CurrentTimestamp - ProneLockTimestamp);
	return FMath::Clamp(RemainingCooldown, 0.f, ProneLockDuration);
}

void USACharacterMovementComponent::SetProneLock(bool bLock)
{
	if (bLock)
	{
		bProneLocked = true;
		ProneLockTimestamp = GetTimestamp();
	}
	else
	{
		bProneLocked = false;
	}
}

float USACharacterMovementComponent::GetTimestamp() const
{
	if (CharacterOwner->GetLocalRole() == ROLE_Authority)
	{
		if (CharacterOwner->IsLocallyControlled())
		{
			// Server owned character
			return GetWorld()->GetTimeSeconds();
		}
		else
		{
			// Server remote character
			const FNetworkPredictionData_Server_Character* ServerData = GetPredictionData_Server_Character();
			return ServerData->CurrentClientTimeStamp;
		}
	}
	else
	{
		// Client owned character
		const FNetworkPredictionData_Client_Character* ClientData = GetPredictionData_Client_Character();
		return ClientData->CurrentTimeStamp;
	}
}

bool USACharacterMovementComponent::IsProned() const
{
	return SACharacterOwner && SACharacterOwner->bIsProned;
}

void USACharacterMovementComponent::Prone(bool bClientSimulation)
{
	if (!HasValidData())
	{
		return;
	}

	if (!bClientSimulation && !CanProneInCurrentState())
	{
		return;
	}

	// See if collision is already at desired size.
	if (CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() == PronedHalfHeight &&
		CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleRadius() == PronedRadius)
	{
		if (!bClientSimulation)
		{
			SACharacterOwner->bIsProned = true;
		}
		SACharacterOwner->OnStartProne(0.f, 0.f);
		SetProneLock(true);
		return;
	}

	if (bClientSimulation && CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)
	{
		// restore collision size before prone
		const ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
		bShrinkProxyCapsule = true;
	}

	// Change collision size to prone dimensions
	const float ComponentScale = CharacterOwner->GetCapsuleComponent()->GetShapeScale();
	const float OldUnscaledHalfHeight = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	const float OldUnscaledRadius = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleRadius();

	// Height is not allowed to be smaller than radius.
	const float ClampedPronedHalfHeight = FMath::Max3(0.f, PronedRadius, PronedHalfHeight);
	CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(PronedRadius, ClampedPronedHalfHeight);
	float HalfHeightAdjust = (OldUnscaledHalfHeight - ClampedPronedHalfHeight);
	float ScaledHalfHeightAdjust = HalfHeightAdjust * ComponentScale;

	if (!bClientSimulation)
	{
		// Proned to a larger height? (this is rare)
		if (ClampedPronedHalfHeight > OldUnscaledHalfHeight)
		{
			FCollisionQueryParams CapsuleParams(SCENE_QUERY_STAT(ProneTrace), false, CharacterOwner);
			FCollisionResponseParams ResponseParam;
			InitCollisionParams(CapsuleParams, ResponseParam);
			const bool bEncroached = GetWorld()->OverlapBlockingTestByChannel(UpdatedComponent->GetComponentLocation() - FVector(0.f, 0.f, ScaledHalfHeightAdjust), FQuat::Identity,
				UpdatedComponent->GetCollisionObjectType(), GetPawnCapsuleCollisionShape(SHRINK_None), CapsuleParams, ResponseParam);

			// If encroached, cancel
			if (bEncroached)
			{
				CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(OldUnscaledRadius, OldUnscaledHalfHeight);
				return;
			}
		}

		if (bCrouchMaintainsBaseLocation)
		{
			// Intentionally not using MoveUpdatedComponent, where a horizontal plane constraint would prevent the base of the capsule from staying at the same spot.
			UpdatedComponent->MoveComponent(FVector(0.f, 0.f, -ScaledHalfHeightAdjust), UpdatedComponent->GetComponentQuat(), true, nullptr, EMoveComponentFlags::MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);
		}

		SACharacterOwner->bIsProned = true;
	}

	// Our capsule is growing during prone, test for encroaching from radius
	FCollisionQueryParams CapsuleParams(SCENE_QUERY_STAT(ProneTrace), false, CharacterOwner);
	FCollisionResponseParams ResponseParam;
	InitCollisionParams(CapsuleParams, ResponseParam);
	FHitResult Hit;
	const FVector Start = UpdatedComponent->GetComponentLocation() - FVector(0.f, 0.f, ScaledHalfHeightAdjust);
	const FVector End = UpdatedComponent->GetComponentLocation() - FVector(0.f, 0.f, ScaledHalfHeightAdjust * 1.01f);
	if (GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, UpdatedComponent->GetCollisionObjectType(), FCollisionShape::MakeCapsule(PronedRadius, PronedHalfHeight), CapsuleParams, ResponseParam))
	{
		if (Hit.bStartPenetrating)
		{
			HandleImpact(Hit);
			SlideAlongSurface(FVector::DownVector, 1.f, Hit.Normal, Hit, true);

			if (Hit.bStartPenetrating)
			{
				OnCharacterStuckInGeometry(&Hit);
			}
		}
	}

	bForceNextFloorCheck = true;

	SetProneLock(true);

	// OnStartProne takes the change from the Default size, not the current one (though they are usually the same).
	const float MeshAdjust = ScaledHalfHeightAdjust;
	const ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
	HalfHeightAdjust = (DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - ClampedPronedHalfHeight);
	ScaledHalfHeightAdjust = HalfHeightAdjust * ComponentScale;

	AdjustProxyCapsuleSize();
	SACharacterOwner->OnStartProne(HalfHeightAdjust, ScaledHalfHeightAdjust);

	// Don't smooth this change in mesh position
	if ((bClientSimulation && CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy) || (IsNetMode(NM_ListenServer) && CharacterOwner->GetRemoteRole() == ROLE_AutonomousProxy))
	{
		FNetworkPredictionData_Client_Character* ClientData = GetPredictionData_Client_Character();
		if (ClientData)
		{
			ClientData->MeshTranslationOffset -= FVector(0.f, 0.f, MeshAdjust);
			ClientData->OriginalMeshTranslationOffset = ClientData->MeshTranslationOffset;
		}
	}
}

void USACharacterMovementComponent::UnProne(bool bClientSimulation)
{
	if (!HasValidData())
	{
		return;
	}

	if (IsProneLocked())
	{
		return;
	}

	ASAPlayableCharacter* DefaultCharacter = SACharacterOwner->GetClass()->GetDefaultObject<ASAPlayableCharacter>();

	// See if collision is already at desired size.
	if (CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() == DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() &&
		CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleRadius() == DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius())
	{
		if (!bClientSimulation)
		{
			SACharacterOwner->bIsProned = false;
		}
		SACharacterOwner->OnEndProne(0.f, 0.f);
		return;
	}

	const float CurrentPronedHalfHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	const float ComponentScale = CharacterOwner->GetCapsuleComponent()->GetShapeScale();
	const float OldUnscaledHalfHeight = CharacterOwner->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	const float HalfHeightAdjust = DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - OldUnscaledHalfHeight;
	const float ScaledHalfHeightAdjust = HalfHeightAdjust * ComponentScale;
	const FVector PawnLocation = UpdatedComponent->GetComponentLocation();

	// Grow to unproned size.
	check(CharacterOwner->GetCapsuleComponent());

	if (!bClientSimulation)
	{
		// Try to stay in place and see if the larger capsule fits. We use a slightly taller capsule to avoid penetration.
		const UWorld* MyWorld = GetWorld();
		constexpr float SweepInflation = UE_KINDA_SMALL_NUMBER * 10.f;
		FCollisionQueryParams CapsuleParams(SCENE_QUERY_STAT(ProneTrace), false, CharacterOwner);
		FCollisionResponseParams ResponseParam;
		InitCollisionParams(CapsuleParams, ResponseParam);

		// Compensate for the difference between current capsule size and standing size
		const FCollisionShape StandingCapsuleShape = GetPawnCapsuleCollisionShape(SHRINK_HeightCustom, -SweepInflation - ScaledHalfHeightAdjust); // Shrink by negative amount, so actually grow it.
		const ECollisionChannel CollisionChannel = UpdatedComponent->GetCollisionObjectType();
		bool bEncroached = true;

		if (!bCrouchMaintainsBaseLocation)
		{
			// Expand in place
			bEncroached = MyWorld->OverlapBlockingTestByChannel(PawnLocation, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);

			if (bEncroached)
			{
				// Try adjusting capsule position to see if we can avoid encroachment.
				if (ScaledHalfHeightAdjust > 0.f)
				{
					// Shrink to a short capsule, sweep down to base to find where that would hit something, and then try to stand up from there.
					float PawnRadius, PawnHalfHeight;
					CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleSize(PawnRadius, PawnHalfHeight);
					const float ShrinkHalfHeight = PawnHalfHeight - PawnRadius;
					const float TraceDist = PawnHalfHeight - ShrinkHalfHeight;
					const FVector Down = FVector(0.f, 0.f, -TraceDist);

					FHitResult Hit(1.f);
					const FCollisionShape ShortCapsuleShape = GetPawnCapsuleCollisionShape(SHRINK_HeightCustom, ShrinkHalfHeight);
					MyWorld->SweepSingleByChannel(Hit, PawnLocation, PawnLocation + Down, FQuat::Identity, CollisionChannel, ShortCapsuleShape, CapsuleParams);
					if (Hit.bStartPenetrating)
					{
						bEncroached = true;
					}
					else
					{
						// Compute where the base of the sweep ended up, and see if we can stand there
						const float DistanceToBase = (Hit.Time * TraceDist) + ShortCapsuleShape.Capsule.HalfHeight;
						const FVector NewLoc = FVector(PawnLocation.X, PawnLocation.Y, PawnLocation.Z - DistanceToBase + StandingCapsuleShape.Capsule.HalfHeight + SweepInflation + MIN_FLOOR_DIST / 2.f);
						bEncroached = MyWorld->OverlapBlockingTestByChannel(NewLoc, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);
						if (!bEncroached)
						{
							// Intentionally not using MoveUpdatedComponent, where a horizontal plane constraint would prevent the base of the capsule from staying at the same spot.
							UpdatedComponent->MoveComponent(NewLoc - PawnLocation, UpdatedComponent->GetComponentQuat(), false, nullptr, EMoveComponentFlags::MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);
						}
					}
				}
			}
		}
		else
		{
			// Expand while keeping base location the same.
			FVector StandingLocation = PawnLocation + FVector(0.f, 0.f, StandingCapsuleShape.GetCapsuleHalfHeight() - CurrentPronedHalfHeight);
			bEncroached = MyWorld->OverlapBlockingTestByChannel(StandingLocation, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);

			if (bEncroached)
			{
				if (IsMovingOnGround())
				{
					// Something might be just barely overhead, try moving down closer to the floor to avoid it.
					constexpr float MinFloorDist = UE_KINDA_SMALL_NUMBER * 10.f;
					if (CurrentFloor.bBlockingHit && CurrentFloor.FloorDist > MinFloorDist)
					{
						StandingLocation.Z -= CurrentFloor.FloorDist - MinFloorDist;
						bEncroached = MyWorld->OverlapBlockingTestByChannel(StandingLocation, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParam);
					}
				}
			}

			if (!bEncroached)
			{
				// Commit the change in location.
				UpdatedComponent->MoveComponent(StandingLocation - PawnLocation, UpdatedComponent->GetComponentQuat(), false, nullptr, EMoveComponentFlags::MOVECOMP_NoFlags, ETeleportType::TeleportPhysics);
				bForceNextFloorCheck = true;
			}
		}

		// If still encroached then abort.
		if (bEncroached)
		{
			return;
		}

		SACharacterOwner->bIsProned = false;
	}
	else
	{
		bShrinkProxyCapsule = true;
	}

	// Now call SetCapsuleSize() to cause touch/untouch events and actually grow the capsule
	CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), true);

	const float MeshAdjust = ScaledHalfHeightAdjust;
	AdjustProxyCapsuleSize();
	SACharacterOwner->OnEndProne(HalfHeightAdjust, ScaledHalfHeightAdjust);

	// Don't smooth this change in mesh position
	if ((bClientSimulation && CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy) || (IsNetMode(NM_ListenServer) && CharacterOwner->GetRemoteRole() == ROLE_AutonomousProxy))
	{
		FNetworkPredictionData_Client_Character* ClientData = GetPredictionData_Client_Character();
		if (ClientData)
		{
			ClientData->MeshTranslationOffset += FVector(0.f, 0.f, MeshAdjust);
			ClientData->OriginalMeshTranslationOffset = ClientData->MeshTranslationOffset;
		}
	}
}

bool USACharacterMovementComponent::CanProneInCurrentState() const
{
	return (IsFalling() || IsMovingOnGround()) && UpdatedComponent && !UpdatedComponent->IsSimulatingPhysics();
}

void USACharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void USACharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	bWantsToProne = ((Flags & FSavedMove_SA::FLAG_Prone) != 0);
}

void USACharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	if (MovementMode == MOVE_Walking && Safe_bWantsToSlide && !bWantsToCrouch)
	{
		FHitResult PotentialSlideSurface;
		if (CanSlide())
		{
			SetMovementMode(MOVE_Custom, CMOVE_Slide);
		}
		//else
		//{
		//	Safe_bWantsToSlide = false;
		//}
	}

	if (IsCustomMovementMode(CMOVE_Slide) && !Safe_bWantsToSlide)	
	{
		SetMovementMode(MOVE_Walking);
	}

	// Proxies get replicated crouch state.
	if (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)
	{
		// Check for a change in crouch state. Players toggle crouch by changing bWantsToCrouch.
		const bool bIsCrouching = IsCrouching();
		if (bIsCrouching && (!bWantsToCrouch || !CanCrouchInCurrentState()))
		{
			UnCrouch(false);
		}
		else if (!bIsCrouching && bWantsToCrouch && CanCrouchInCurrentState())
		{
			if (IsProned())
			{
				bWantsToProne = false;
				UnProne(false);
			}
			if (!IsProned())
			{
				// Potential prone lock
				Crouch(false);
			}
		}
	}

	// Proxies get replicated Prone state.
	if (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)
	{
		// Check if prone lock timer has expired
		if (bProneLocked && !IsProneLockOnTimer())
		{
			SetProneLock(false);
		}

		// Check for a change in Prone state. Players toggle Prone by changing bWantsToProne.
		const bool bIsProned = IsProned();
		if (bIsProned && (!bWantsToProne || !CanProneInCurrentState()))
		{
			UnProne(false);
		}
		else if (!bIsProned && bWantsToProne && CanProneInCurrentState())
		{
			if (IsCrouching())
			{
				bWantsToCrouch = false;
				UnCrouch(false);
			}
			Prone(false);
		}
	}
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void USACharacterMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);

	// Proxies get replicated Prone state.
	if (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)
	{
		// UnProne if no longer allowed to be Proned
		if (IsProned() && !CanProneInCurrentState())
		{
			UnProne(false);
		}
	}
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

void USACharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Slide) Safe_ExitSlide();
	if (IsCustomMovementMode(CMOVE_Slide)) Safe_EnterSlide(PreviousMovementMode, (ECustomMovementMode)PreviousCustomMode);
}

bool USACharacterMovementComponent::ClientUpdatePositionAfterServerUpdate()
{
	const bool bRealProne = bWantsToProne;
	const bool bResult = Super::ClientUpdatePositionAfterServerUpdate();
	bWantsToProne = bRealProne;

	return bResult;
}

void USACharacterMovementComponent::Safe_EnterSlide(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode)
{
	bWantsToCrouch = true;	// 캡슐 줄일거임

	Velocity += Velocity.GetSafeNormal2D() * SlideEnterImpulse;
	SetMovementMode(MOVE_Custom, CMOVE_Slide);

	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, NULL);

	UE_LOG(LogTemp, Warning, TEXT("Slide Start"));
}

void USACharacterMovementComponent::Safe_ExitSlide()
{
	bWantsToCrouch = false;

	Safe_bWantsToSlide = false;

	UE_LOG(LogTemp, Warning, TEXT("Slide End"));
}

void USACharacterMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}	
	
	if (!CanSlide())
	{
		SetMovementMode(MOVE_Walking);
		StartNewPhysics(deltaTime, Iterations);	
		return;
	}

	bJustTeleported = false;
	bool bCheckedFall = false;
	bool bTriedLedgeMove = false;
	float remainingTime = deltaTime;

	// Perform the move
	while ((remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)))
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;

		// Save current values
		UPrimitiveComponent* const OldBase = GetMovementBase();
		const FVector PreviousBaseLocation = (OldBase != NULL) ? OldBase->GetComponentLocation() : FVector::ZeroVector;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		const FFindFloorResult OldFloor = CurrentFloor;

		// Ensure velocity is horizontal.
		MaintainHorizontalGroundVelocity();
		const FVector OldVelocity = Velocity;

		FVector SlopeForce = CurrentFloor.HitResult.Normal;
		SlopeForce.Z = 0.f;
		Velocity += SlopeForce * SlideGravityForce * deltaTime;

		Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector().GetSafeNormal2D());

		// Apply acceleration
		CalcVelocity(timeTick, GroundFriction * SlideFrictionFactor, false, GetMaxBrakingDeceleration());

		// Compute move parameters
		const FVector MoveVelocity = Velocity;
		const FVector Delta = timeTick * MoveVelocity;
		const bool bZeroDelta = Delta.IsNearlyZero();
		FStepDownResult StepDownResult;
		bool bFloorWalkable = CurrentFloor.IsWalkableFloor();

		if (bZeroDelta)
		{
			remainingTime = 0.f;
		}
		else
		{
			// try to move forward
			MoveAlongFloor(MoveVelocity, timeTick, &StepDownResult);

			if (IsFalling())
			{
				// pawn decided to jump up
				const float DesiredDist = Delta.Size();
				if (DesiredDist > KINDA_SMALL_NUMBER)
				{
					const float ActualDist = (UpdatedComponent->GetComponentLocation() - OldLocation).Size2D();
					remainingTime += timeTick * (1.f - FMath::Min(1.f, ActualDist / DesiredDist));
				}
				StartNewPhysics(remainingTime, Iterations);
				return;
			}
			else if (IsSwimming()) //just entered water
			{
				StartSwimming(OldLocation, OldVelocity, timeTick, remainingTime, Iterations);
				return;
			}
		}

		// Update floor.
		// StepUp might have already done it for us.
		if (StepDownResult.bComputedFloor)
		{
			CurrentFloor = StepDownResult.FloorResult;
		}
		else
		{
			FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, bZeroDelta, NULL);
		}


		// check for ledges here
		const bool bCheckLedges = !CanWalkOffLedges();
		if (bCheckLedges && !CurrentFloor.IsWalkableFloor())
		{
			// calculate possible alternate movement
			const FVector GravDir = FVector(0.f, 0.f, -1.f);
			const FVector NewDelta = bTriedLedgeMove ? FVector::ZeroVector : GetLedgeMove(OldLocation, Delta, GravDir);
			if (!NewDelta.IsZero())
			{
				// first revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, false);

				// avoid repeated ledge moves if the first one fails
				bTriedLedgeMove = true;

				// Try new movement direction
				Velocity = NewDelta / timeTick;
				remainingTime += timeTick;
				continue;
			}
			else
			{
				// see if it is OK to jump
				// @todo collision : only thing that can be problem is that oldbase has world collision on
				bool bMustJump = bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump))
				{
					return;
				}
				bCheckedFall = true;

				// revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, true);
				remainingTime = 0.f;
				break;
			}
		}
		else
		{
			// Validate the floor check
			if (CurrentFloor.IsWalkableFloor())
			{
				if (ShouldCatchAir(OldFloor, CurrentFloor))
				{
					HandleWalkingOffLedge(OldFloor.HitResult.ImpactNormal, OldFloor.HitResult.Normal, OldLocation, timeTick);
					if (IsMovingOnGround())
					{
						// If still walking, then fall. If not, assume the user set a different mode they want to keep.
						StartFalling(Iterations, remainingTime, timeTick, Delta, OldLocation);
					}
					return;
				}

				AdjustFloorHeight();
				SetBase(CurrentFloor.HitResult.Component.Get(), CurrentFloor.HitResult.BoneName);
			}
			else if (CurrentFloor.HitResult.bStartPenetrating && remainingTime <= 0.f)
			{
				// The floor check failed because it started in penetration
				// We do not want to try to move downward because the downward sweep failed, rather we'd like to try to pop out of the floor.
				FHitResult Hit(CurrentFloor.HitResult);
				Hit.TraceEnd = Hit.TraceStart + FVector(0.f, 0.f, MAX_FLOOR_DIST);
				const FVector RequestedAdjustment = GetPenetrationAdjustment(Hit);
				ResolvePenetration(RequestedAdjustment, Hit, UpdatedComponent->GetComponentQuat());
				bForceNextFloorCheck = true;
			}

			// check if just entered water
			if (IsSwimming())
			{
				StartSwimming(OldLocation, Velocity, timeTick, remainingTime, Iterations);
				return;
			}

			// See if we need to start falling.
			if (!CurrentFloor.IsWalkableFloor() && !CurrentFloor.HitResult.bStartPenetrating)
			{
				const bool bMustJump = bJustTeleported || bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump))
				{
					return;
				}
				bCheckedFall = true;
			}
		}

		// Allow overlap events and such to change physics state and velocity
		if (IsMovingOnGround() && bFloorWalkable)
		{
			// Make velocity reflect actual move
			if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && timeTick >= MIN_TICK_TIME)
			{
				// TODO-RootMotionSource: Allow this to happen during partial override Velocity, but only set allowed axes?
				Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick;
				MaintainHorizontalGroundVelocity();
			}
		}

		// If we didn't move at all this iteration then abort (since future iterations will also be stuck).
		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}
	}

	FHitResult Hit;
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(Velocity.GetSafeNormal2D(), FVector::UpVector).ToQuat();
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, false, Hit);
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
