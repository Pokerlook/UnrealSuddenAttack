// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SA/Input/InputConfig.h"
#include "InputActionValue.h"
#include "SAPlayerController.generated.h"

class UInputMappingContext;
class ICommandInterface;


/**
 * 
 */
UCLASS()
class SA_API ASAPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** The input config that maps Input Actions to Input Tags*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputConfig* InputConfig;
	

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;


private:
	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY()
		TScriptInterface<ICommandInterface> CharacterCommand;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Crouch(const FInputActionValue& Value);
	void Prone(const FInputActionValue& Value);
	void Sneek(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void Aim(const FInputActionValue& Value);
	void Wheel(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);
	void HolsterWeapon(const FInputActionValue& Value);
	void ChangeWeaponTo1(const FInputActionValue& Value);
	void ChangeWeaponTo2(const FInputActionValue& Value);
	void ChangeWeaponTo3(const FInputActionValue& Value);
	void ChangeWeaponTo4(const FInputActionValue& Value);
	void ChangeWeaponTo5(const FInputActionValue& Value);
	void LeanLeft(const FInputActionValue& Value);
	void LeanRight(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void ShootMode(const FInputActionValue& Value);
	void ViewMode(const FInputActionValue& Value);
	void DropWeapon(const FInputActionValue& Value);
	void Vault(const FInputActionValue& Value);
	void FreeLook(const FInputActionValue& Value);

	void Inventory(const FInputActionValue& Value);
	void Map(const FInputActionValue& Value);
	void MiniMap(const FInputActionValue& Value);
	void ESC(const FInputActionValue& Value);
	void Leaderboard(const FInputActionValue& Value);
	void WeaponMenu(const FInputActionValue& Value);

};
