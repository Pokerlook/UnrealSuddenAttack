// Fill out your copyright notice in the Description page of Project Settings.


#include "SA/Character/SAPlayableCharacter.h"
#include "SA/Player/SAPlayerState.h"
#include "SA/SATagSingleton.h"
#include "SA/Component/SAInventoryComponent.h"
#include "SA/Interface/InteractInterface.h"
#include "SA/Component/SACharacterMovementComponent.h"
#include "SA/SABlueprintFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

ASAPlayableCharacter::ASAPlayableCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SACharacterMovementComponent = Cast<USACharacterMovementComponent>(GetCharacterMovement());

	// Configure character movement	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->SetCrouchedHalfHeight(70.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 150.f;	// if zoom, 75. zoom mode, change camera?
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->ProbeSize = 6.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	InventoryComponent = CreateDefaultSubobject<USAInventoryComponent>(TEXT("Inventory"));
	InventoryComponent->SetIsReplicated(true);

	GetCapsuleComponent()->SetCapsuleHalfHeight(95.f);
	GetCapsuleComponent()->SetCapsuleRadius(20.f);

}

EWeaponType ASAPlayableCharacter::GetEquippedWeaponType() const
{	
	return InventoryComponent->GetWeaponType();
}

void ASAPlayableCharacter::MoveCommand(FVector2D Value)
{
	if (Controller == nullptr) return;

	if (USABlueprintFunctionLibrary::IsFullBodyMontagePlaying(GetMesh()->GetAnimInstance()))
	{
		// 풀바디 몽타주 실행중엔 움직이지 않는다
		return;
	}

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// check is go front or not. check is sneek, walk, or sprint. change max speed.

	if (!bIsProned)
	{
		// add movement 
		AddMovementInput(ForwardDirection, Value.Y);
		AddMovementInput(RightDirection, Value.X);
		return;
	}

	const FSAGameplayTags& GameplayTags = FSAGameplayTags::Get();
	FGameplayTagContainer OwingTags;
	AbilitySystemComponent->GetOwnedGameplayTags(OwingTags);
	if (!OwingTags.HasTag(GameplayTags.State_Stance_Prone))
	{
		UE_LOG(LogTemp, Error, TEXT("bIsProne is True, but no State_Stance_Prone tag"));
		return;
	}

	// if prone, check ray trace
	if (Value.Y > 0) // go front
	{
		if (!IsPathClear(GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * (StandHeight * 1.1f)))) return;

		FVector Start = GetActorLocation() + (GetActorForwardVector() * (StandHeight + 1));
		FVector End = Start - FVector::ZAxisVector* StandHeight;
		if (IsPathClear(Start, End))
		{
			ProneCommand();
			return;
		}
	}
	else if(Value.Y < 0)// go back
	{
		if (!IsPathClear(GetActorLocation(), GetActorLocation() - (GetActorForwardVector() * (StandHeight * 1.1f)))) return;

		FVector Start = GetActorLocation() - (GetActorForwardVector() * (StandHeight + 1));
		FVector End = Start - FVector::ZAxisVector * StandHeight;
		if (IsPathClear(Start, End))
		{
			ProneCommand();
			return;
		}
	}

	if (Value.X > 0) // go right
	{
		if (!IsPathClear(GetActorLocation(), GetActorLocation() + (GetActorRightVector() * (StandRadius * 1.3f)))) return;

		FVector Start = GetActorLocation() + (GetActorRightVector() * (StandRadius * 1.3f));
		FVector End = Start - FVector::ZAxisVector * StandHeight;
		if (IsPathClear(Start, End))
		{
			ProneCommand();
			return;
		}
	}
	else if(Value.X < 0) // go left
	{
		if (!IsPathClear(GetActorLocation(), GetActorLocation() - (GetActorRightVector() * (StandRadius * 1.3f)))) return;

		FVector Start = GetActorLocation() - (GetActorRightVector() * (StandRadius * 1.3f));
		FVector End = Start - FVector::ZAxisVector * StandHeight;
		if (IsPathClear(Start, End))
		{
			ProneCommand();
			return;
		}
	}

	// add movement 
	AddMovementInput(ForwardDirection, Value.Y);
	AddMovementInput(RightDirection, Value.X);
}

bool ASAPlayableCharacter::IsPathClear(FVector Start, FVector End)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams = GetIgnoreCharacterParams();

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,          // 결과를 저장할 HitResult
		Start,              // 시작 지점
		End,                // 끝 지점
		ECC_Pawn,           // 충돌 채널
		CollisionParams     // 충돌 파라미터
	);
#if WITH_EDITOR
	FColor LineColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugLine(GetWorld(), Start, End, LineColor, false, 1.0f, 0, 1.0f);
#endif
	return !bHit;	// 부딪힌게 없다 
}


void ASAPlayableCharacter::LookCommand(FVector2D Value)
{
	AddControllerYawInput(Value.X); 
	AddControllerPitchInput(-Value.Y);
}

void ASAPlayableCharacter::JumpCommand(bool Value)
{
	if (Value)
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FSAGameplayTags::Get().State_Stance_Crouch)
			|| AbilitySystemComponent->HasMatchingGameplayTag(FSAGameplayTags::Get().State_Stance_Prone))
		{
			return;
		}
		AbilityStart(FSAGameplayTags::Get().InputTag_Jump);
	}
	else
	{
		AbilityEnd(FSAGameplayTags::Get().InputTag_Jump);
	}
}

void ASAPlayableCharacter::InteractCommand(bool Value)
{
	if (Value == true && !ThisInteract) return;
	if (Value == false)
	{
		if (!ThisInteract) return;
		ThisInteract->InteractEnd(this);
		isInteracting = false;
	}
	if (Value == true)
	{
		if (!ThisInteract) return;
		ThisInteract->InteractStart(this); 
		UE_LOG(LogTemp, Warning, TEXT("%s : InteractStart "), *ThisInteract.GetObject()->GetName());
		isInteracting = true;
	}
}

void ASAPlayableCharacter::SprintCommand(bool Value)
{
	SACharacterMovementComponent->Slide();
}

void ASAPlayableCharacter::CrouchCommand()
{
	const FGameplayTag CrouchTag = FSAGameplayTags::Get().InputTag_Crouch;
	AbilityStart(CrouchTag);
}

void ASAPlayableCharacter::ProneCommand()
{
	const FGameplayTag ProneTag = FSAGameplayTags::Get().InputTag_Prone;
	AbilityStart(ProneTag);
}

FTransform ASAPlayableCharacter::GetWeaponLeftHandSocketTransform() const
{
	return InventoryComponent->GetWeaponLeftHandSocketTransform();
}

FCollisionQueryParams ASAPlayableCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;

	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}

void ASAPlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckInteractInterface();

	if (!bIsProned) return;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()*2); // Adjust the trace distance as needed
	FHitResult HitResult;

	FCollisionQueryParams Params = GetIgnoreCharacterParams();

	// Perform a line trace (raycast) to detect the floor
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	if (bHit)
	{
		FVector FloorNormal = HitResult.ImpactNormal;

		// Calculate the new rotation
		FRotator NewRotation = FRotationMatrix::MakeFromZX(FloorNormal, GetActorForwardVector()).Rotator();

		// Optionally, preserve the yaw (rotation around Z axis)
		NewRotation.Yaw = GetActorRotation().Yaw;

		// Apply the new rotation
		SetActorRotation(NewRotation);
	}

}

void ASAPlayableCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
	AddCharacterAbilities();
	InventoryComponent->InitInventory(GetAbilitySystemComponent());
	BindEventCallback();
}

void ASAPlayableCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void ASAPlayableCharacter::OnRep_AbilitySystemComponent()
{
	InventoryComponent->InitInventory(AbilitySystemComponent);
	BindEventCallback();
}

void ASAPlayableCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (!AbilitySystemComponent) return;

	const FSAGameplayTags& GameplayTags = FSAGameplayTags::Get();
	FGameplayTagContainer TagContainer; 
	TagContainer.AddTag(GameplayTags.State_InAir_Jumping);

	AbilitySystemComponent->RemoveActiveEffectsWithTags(TagContainer);

}

void ASAPlayableCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASAPlayableCharacter, InventoryComponent);
	DOREPLIFETIME_CONDITION(ThisClass, bIsProned, COND_SimulatedOnly);
}

void ASAPlayableCharacter::RecalculateBaseEyeHeight()
{
	if (bIsProned)
	{
		BaseEyeHeight = PronedEyeHeight;
	}
	else
	{
		Super::RecalculateBaseEyeHeight();
	}
}

void ASAPlayableCharacter::FaceRotation(FRotator ControlRotation, float DeltaTime)
{
	Super::FaceRotation(ControlRotation, DeltaTime);

}

void ASAPlayableCharacter::Prone(bool bClientSimulation)
{
	if (SACharacterMovementComponent)
	{
		if (CanProne())
		{
			SACharacterMovementComponent->bWantsToProne = true;
		}
	}
}

void ASAPlayableCharacter::UnProne(bool bClientSimulation)
{
	if (SACharacterMovementComponent)
	{
		SACharacterMovementComponent->bWantsToProne = false;
	}
}

bool ASAPlayableCharacter::CanProne() const
{
	return !bIsProned && GetRootComponent() && !GetRootComponent()->IsSimulatingPhysics();
}

void ASAPlayableCharacter::OnEndProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	RecalculateBaseEyeHeight();

	if (!bIsCrouched)
	{
		const ACharacter* DefaultChar = GetDefault<ACharacter>(GetClass());
		if (GetMesh() && DefaultChar->GetMesh())
		{
			FVector& MeshRelativeLocation = GetMesh()->GetRelativeLocation_DirectMutable();
			MeshRelativeLocation.Z = DefaultChar->GetMesh()->GetRelativeLocation().Z;
			BaseTranslationOffset.Z = MeshRelativeLocation.Z;
		}
		else
		{
			BaseTranslationOffset.Z = DefaultChar->GetBaseTranslationOffset().Z;
		}
	}
	K2_OnEndProne(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void ASAPlayableCharacter::OnStartProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	RecalculateBaseEyeHeight();

	const ACharacter* DefaultChar = GetDefault<ACharacter>(GetClass());
	if (GetMesh() && DefaultChar->GetMesh())
	{
		FVector& MeshRelativeLocation = GetMesh()->GetRelativeLocation_DirectMutable();
		MeshRelativeLocation.Z = DefaultChar->GetMesh()->GetRelativeLocation().Z + HalfHeightAdjust;
		BaseTranslationOffset.Z = MeshRelativeLocation.Z;
	}
	else
	{
		BaseTranslationOffset.Z = DefaultChar->GetBaseTranslationOffset().Z + HalfHeightAdjust;
	}

	K2_OnStartProne(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void ASAPlayableCharacter::OnRep_IsProned()
{
	if (SACharacterMovementComponent)
	{
		if (bIsProned)
		{
			SACharacterMovementComponent->bWantsToProne = true;
			SACharacterMovementComponent->Prone(true);
		}
		else
		{
			SACharacterMovementComponent->bWantsToProne = false;
			SACharacterMovementComponent->UnProne(true);
		}
		SACharacterMovementComponent->bNetworkUpdateReceived = true;
	}
}

void ASAPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	StandHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	StandRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
}

void ASAPlayableCharacter::InitAbilityActorInfo()
{
	ASAPlayerState* PS = GetPlayerStateChecked<ASAPlayerState>();
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	AbilitySystemComponent = PS->GetAbilitySystemComponent();
	AttributeSet = PS->GetAttributeSet();
}

void ASAPlayableCharacter::BindEventCallback()
{
	const FSAGameplayTags& GameplayTags = FSAGameplayTags::Get();
	AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(GameplayTags.Event_Locomotion_Crouch).AddUObject(this, &ASAPlayableCharacter::StanceEventCallback);
	AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(GameplayTags.Event_Locomotion_Stand).AddUObject(this, &ASAPlayableCharacter::StanceEventCallback);
	AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(GameplayTags.Event_Locomotion_Prone).AddUObject(this, &ASAPlayableCharacter::StanceEventCallback);

}

void ASAPlayableCharacter::StanceEventCallback(const FGameplayEventData* Payload)
{
	const FSAGameplayTags& GameplayTags = FSAGameplayTags::Get();
	FGameplayTagContainer OwingTags;
	AbilitySystemComponent->GetOwnedGameplayTags(OwingTags);

	if (OwingTags.HasTag(GameplayTags.State_Stance_Crouch))
	{
		SetCharacterStance(ECharacterStance::Crouch);

		// change movement's max walk speed
	}
	else if (OwingTags.HasTag(GameplayTags.State_Stance_Prone))
	{
		SetCharacterStance(ECharacterStance::Prone);
		// set capsule
		
		// change movement's max walk speed
	}
	else
	{
		SetCharacterStance(ECharacterStance::Stand);
		// set capsule
		 
		// change movement's max walk speed
	}
	
}

void ASAPlayableCharacter::AbilityStart(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return; 

	for (FGameplayAbilitySpec& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySystemComponent->AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				AbilitySystemComponent->TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}

}

void ASAPlayableCharacter::AbilityEnd(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySystemComponent->AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void ASAPlayableCharacter::CheckInteractInterface()
{
	if (!IsLocallyControlled()) return;

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + FollowCamera->GetForwardVector() * 1000.0f;
	FHitResult HitResult;

	FCollisionQueryParams TraceParams(FName(TEXT("CommandTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

	if (!bHit && !ThisInteract) return;
	if (isInteracting) return;	// interact중이면 ThisInteract 가지고 있어야 하니 아래 코드 실행 안 함
	LastInteract = ThisInteract;
	ThisInteract = HitResult.GetActor();

	if (LastInteract != ThisInteract)
	{
		if (LastInteract) LastInteract->HideInteractWidget();
		if (ThisInteract) ThisInteract->ShowInteractWidget();
	}
}
