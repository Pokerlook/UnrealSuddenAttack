// Fill out your copyright notice in the Description page of Project Settings.


#include "SA/Character/SAPlayableCharacter.h"
#include "SA/Player/SAPlayerState.h"
#include "SA/SATagSingleton.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"

ASAPlayableCharacter::ASAPlayableCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Configure character movement	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 150.f;	// if zoom, 75. zoom mode, change camera?
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->ProbeSize = 6.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ASAPlayableCharacter::MoveCommand(FVector2D Value)
{
	if (Controller == nullptr) return;

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

void ASAPlayableCharacter::LookCommand(FVector2D Value)
{
	//if (APlayerController* PC = Cast<APlayerController>(Controller))
	//{		
	//	PC->AddYawInput(Value.X);
	//	PC->AddPitchInput(Value.Y);
	//}
	AddControllerYawInput(Value.X); 
	AddControllerPitchInput(-Value.Y);
}

void ASAPlayableCharacter::JumpCommand(bool Value)
{
	if (Value)
	{
		// Later, should check crouch and prone, return to stand.
		// if stand, normal jump
		const FGameplayTag JumpTag = FSAGameplayTags::Get().InputTag_Jump;
		AbilityStart(JumpTag);
		//UE_LOG(LogTemp, Warning, TEXT("JumpAbility TryActivateAbility"));
	}
	else
	{
		AbilityEnd(FSAGameplayTags::Get().InputTag_Jump);
	}
}

void ASAPlayableCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void ASAPlayableCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
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

void ASAPlayableCharacter::InitAbilityActorInfo()
{
	ASAPlayerState* PS = GetPlayerStateChecked<ASAPlayerState>();
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	AbilitySystemComponent = PS->GetAbilitySystemComponent();
	AttributeSet = PS->GetAttributeSet();
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
