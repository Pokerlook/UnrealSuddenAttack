// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SA/Character/SACharacterBase.h"
#include "SA/Interface/CommandInterface.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "SAPlayableCharacter.generated.h"

class IInteractInterface;
class USACharacterMovementComponent;
class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class SA_API ASAPlayableCharacter : public ASACharacterBase, public ICommandInterface
{
	GENERATED_BODY()
public:
	ASAPlayableCharacter(const FObjectInitializer& ObjectInitializer);

	virtual EWeaponType GetEquippedWeaponType() const override;
	// Command Interface
	virtual void MoveCommand(FVector2D Value) override;
	virtual void LookCommand(FVector2D Value) override;
	virtual void JumpCommand(bool Value) override;
	virtual void InteractCommand(bool Value) override;
	virtual void CrouchCommand() override;
	virtual void ProneCommand() override;
	// command
	
	// anim
	virtual FTransform GetWeaponLeftHandSocketTransform() const override;
	// anim

	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//UFUNCTION(BlueprintCallable)
	//	void Prone();
	//UFUNCTION(BlueprintCallable)
	//	void UnProne();
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	UFUNCTION(BlueprintPure) FORCEINLINE USACharacterMovementComponent* GetSACharacterMovement() const { return SACharacterMovementComponent; }

	FCollisionQueryParams GetIgnoreCharacterParams() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement) 
		USACharacterMovementComponent* SACharacterMovementComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CapsuleHeightStand;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float CapsuleHeightProne;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float MeshZLocationStand;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float MeshZLocationProne;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CameraZLocationStand;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float CameraZLocationCrouch;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float CameraZLocationProne;

	virtual void BeginPlay() override;

	virtual void OnRep_AbilitySystemComponent() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom")
		void AtProne();	// set prone capsule rotation & location, main capsule size, mesh location
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom")
		void AtCrouch(); // set prone capsule rotation & location, main capsule size, mesh location
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom")
		void AtStand(); // set prone capsule rotation & location, main capsule size, mesh location
private:

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Replicated)
		class USAInventoryComponent* InventoryComponent;	// 인벤토리 인터페이스나 Getter가 필요할 수도

	TScriptInterface<IInteractInterface> ThisInteract;
	TScriptInterface<IInteractInterface> LastInteract;
	bool isInteracting = false;


	float NowMaxSpeed;
	float NowMaxBackSpeed;

	UPROPERTY(EditDefaultsOnly)float MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly)float MaxWalkSpeed_Back;

	UPROPERTY(EditDefaultsOnly)float MaxSprintSpeed;
	UPROPERTY(EditDefaultsOnly)float MaxSprintSpeed_Back;

	UPROPERTY(EditDefaultsOnly)float MaxSneekSpeed;
	UPROPERTY(EditDefaultsOnly)float MaxSneekSpeed_Back;

	void InitAbilityActorInfo();
	void BindEventCallback();
	void StanceEventCallback(const FGameplayEventData* Payload);

	void AbilityStart(const FGameplayTag& InputTag);
	void AbilityEnd(const FGameplayTag& InputTag);

	void CheckInteractInterface();

};
