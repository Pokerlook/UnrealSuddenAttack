// Fill out your copyright notice in the Description page of Project Settings.


#include "SA/Player/SAPlayerController.h"
#include "SA/Input/SAInputComponent.h"
#include "SA/SATagSingleton.h"
#include "SA/Interface/CommandInterface.h"

#include "EnhancedInputSubsystems.h"


void ASAPlayerController::BeginPlay()
{
	Super::BeginPlay();

//	check(InputContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	//check(Subsystem);
	//Subsystem->AddMappingContext(AuraContext, 0); // 멀티에선 없다?
	if (Subsystem)
	{
		Subsystem->AddMappingContext(InputContext, 0);
	}

}

void ASAPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	CharacterCommand = aPawn;

}

void ASAPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const FSAGameplayTags& GameplayTags = FSAGameplayTags::Get();

	// ETriggerEvent::Started , Completed, Triggered
	USAInputComponent* SAInputComp = CastChecked< USAInputComponent>(InputComponent);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ASAPlayerController::Move);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Look, ETriggerEvent::Triggered, this, &ASAPlayerController::Look);

	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Crouch, ETriggerEvent::Started, this, &ASAPlayerController::Crouch);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Prone, ETriggerEvent::Started, this, &ASAPlayerController::Prone);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Wheel, ETriggerEvent::Started, this, &ASAPlayerController::Wheel);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Reload, ETriggerEvent::Started, this, &ASAPlayerController::Reload);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_HolsterWeapon, ETriggerEvent::Started, this, &ASAPlayerController::HolsterWeapon);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Weapon1, ETriggerEvent::Started, this, &ASAPlayerController::ChangeWeaponTo1);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Weapon2, ETriggerEvent::Started, this, &ASAPlayerController::ChangeWeaponTo2);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Weapon3, ETriggerEvent::Started, this, &ASAPlayerController::ChangeWeaponTo3);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Weapon4, ETriggerEvent::Started, this, &ASAPlayerController::ChangeWeaponTo4);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Weapon5, ETriggerEvent::Started, this, &ASAPlayerController::ChangeWeaponTo5);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Interact, ETriggerEvent::Started, this, &ASAPlayerController::Interact);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_ShootMode, ETriggerEvent::Started, this, &ASAPlayerController::ShootMode);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_ViewMode, ETriggerEvent::Started, this, &ASAPlayerController::ViewMode);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_DropWeapon, ETriggerEvent::Started, this, &ASAPlayerController::DropWeapon);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Vault, ETriggerEvent::Started, this, &ASAPlayerController::Vault);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Inventory, ETriggerEvent::Started, this, &ASAPlayerController::Inventory);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Map, ETriggerEvent::Started, this, &ASAPlayerController::Map);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_MiniMap, ETriggerEvent::Started, this, &ASAPlayerController::MiniMap);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_ESC, ETriggerEvent::Started, this, &ASAPlayerController::ESC);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Leaderboard, ETriggerEvent::Started, this, &ASAPlayerController::Leaderboard);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_WeaponMenu, ETriggerEvent::Started, this, &ASAPlayerController::WeaponMenu);

	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Jump, ETriggerEvent::Started, this, &ASAPlayerController::Jump);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Jump, ETriggerEvent::Completed, this, &ASAPlayerController::Jump);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Sneek, ETriggerEvent::Started, this, &ASAPlayerController::Sneek);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Sneek, ETriggerEvent::Completed, this, &ASAPlayerController::Sneek);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Sprint, ETriggerEvent::Started, this, &ASAPlayerController::Sprint);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Sprint, ETriggerEvent::Completed, this, &ASAPlayerController::Sprint);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Fire, ETriggerEvent::Started, this, &ASAPlayerController::Fire);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Fire, ETriggerEvent::Completed, this, &ASAPlayerController::Fire);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Aim, ETriggerEvent::Started, this, &ASAPlayerController::Aim);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_Aim, ETriggerEvent::Completed, this, &ASAPlayerController::Aim);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_LeanLeft, ETriggerEvent::Started, this, &ASAPlayerController::LeanLeft);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_LeanLeft, ETriggerEvent::Completed, this, &ASAPlayerController::LeanLeft);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_LeanRight, ETriggerEvent::Started, this, &ASAPlayerController::LeanRight);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_LeanRight, ETriggerEvent::Completed, this, &ASAPlayerController::LeanRight);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_FreeLook, ETriggerEvent::Started, this, &ASAPlayerController::FreeLook);
	SAInputComp->BindActionByTag(InputConfig, GameplayTags.InputTag_FreeLook, ETriggerEvent::Completed, this, &ASAPlayerController::FreeLook);



}

void ASAPlayerController::Move(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	FVector2D MoveVector = Value.Get<FVector2D>();
	CharacterCommand->MoveCommand(MoveVector);

}

void ASAPlayerController::Look(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	CharacterCommand->LookCommand(LookAxisVector);

	//AddYawInput(LookAxisVector.X);
	//AddPitchInput(LookAxisVector.Y);
}

void ASAPlayerController::Jump(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	bool bValue = Value.Get<bool>();
//	UE_LOG(LogTemp, Warning, TEXT("space Jump %d"), bValue);
	CharacterCommand->JumpCommand(bValue);
}

void ASAPlayerController::Crouch(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("C Crouch"));
}

void ASAPlayerController::Prone(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("Z Prone"));
}

void ASAPlayerController::Sneek(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	bool bValue = Value.Get<bool>();
	UE_LOG(LogTemp, Warning, TEXT("left Ctrl Sneek %d"), bValue);
}

void ASAPlayerController::Sprint(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	bool bValue = Value.Get<bool>();
	UE_LOG(LogTemp, Warning, TEXT("left shift Sprint %d"), bValue);
}

void ASAPlayerController::Fire(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	bool bValue = Value.Get<bool>();
	UE_LOG(LogTemp, Warning, TEXT("LMB Fire %d"), bValue);
}

void ASAPlayerController::Aim(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	bool bValue = Value.Get<bool>();
	UE_LOG(LogTemp, Warning, TEXT("RMB Aim %d"), bValue);
}

void ASAPlayerController::Wheel(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	float WheelValue = Value.Get<float>();
	UE_LOG(LogTemp, Warning, TEXT("Wheel %d"), WheelValue);
}

void ASAPlayerController::Reload(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("R Reload"));
}

void ASAPlayerController::HolsterWeapon(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("X Holster"));
}

void ASAPlayerController::ChangeWeaponTo1(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("1 Weapon1"));
}

void ASAPlayerController::ChangeWeaponTo2(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("2 Weapon2"));
}

void ASAPlayerController::ChangeWeaponTo3(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("3 Weapon3"));
}

void ASAPlayerController::ChangeWeaponTo4(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("4 Weapon4"));
}

void ASAPlayerController::ChangeWeaponTo5(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("5 Weapon5"));
}

void ASAPlayerController::LeanLeft(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	bool bValue = Value.Get<bool>();
	UE_LOG(LogTemp, Warning, TEXT("Q LeanLeft %d"), bValue);
}

void ASAPlayerController::LeanRight(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	bool bValue = Value.Get<bool>();
	UE_LOG(LogTemp, Warning, TEXT("E LeanRight %d"), bValue);
}

void ASAPlayerController::Interact(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("F Interact"));
}

void ASAPlayerController::ShootMode(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("B ShootMode"));
}

void ASAPlayerController::ViewMode(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("V ViewMode"));
}

void ASAPlayerController::DropWeapon(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("G DropWeapon"));
}

void ASAPlayerController::Vault(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("H Vault"));
}

void ASAPlayerController::FreeLook(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	bool bValue = Value.Get<bool>();
	UE_LOG(LogTemp, Warning, TEXT("left Alt FreeLook %d"), bValue);
}

void ASAPlayerController::Inventory(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("I Inventory"));
}

void ASAPlayerController::Map(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("M Map"));
}

void ASAPlayerController::MiniMap(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("N MiniMap"));
}

void ASAPlayerController::ESC(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("ESC ESC"));
}

void ASAPlayerController::Leaderboard(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("Tab Leaderboard"));
}

void ASAPlayerController::WeaponMenu(const FInputActionValue& Value)
{
	if (!CharacterCommand) return;
	UE_LOG(LogTemp, Warning, TEXT("P WeaponMenu (Preset)"));
}
