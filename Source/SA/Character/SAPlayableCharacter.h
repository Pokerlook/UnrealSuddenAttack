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

	/** Set by character movement to specify that this Character is currently Proned. */
	UPROPERTY(BlueprintReadOnly, replicatedUsing = OnRep_IsProned, Category = Character)
		uint32 bIsProned : 1;

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	UFUNCTION(BlueprintPure) FORCEINLINE USACharacterMovementComponent* GetSACharacterMovement() const
	{ return SACharacterMovementComponent; }

	virtual EWeaponType GetEquippedWeaponType() const override;
	// Command Interface
	virtual void MoveCommand(FVector2D Value) override;
	virtual void LookCommand(FVector2D Value) override;
	virtual void JumpCommand(bool Value) override;
	virtual void InteractCommand(bool Value) override;
	virtual void SprintCommand(bool Value) override;
	virtual void CrouchCommand() override;
	virtual void ProneCommand() override;
	// command
	
	// anim
	virtual FTransform GetWeaponLeftHandSocketTransform() const override;
	// anim

	FCollisionQueryParams GetIgnoreCharacterParams() const;

	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void RecalculateBaseEyeHeight() override;

	/**
	 * Request the character to start Proned. The request is processed on the next update of the CharacterMovementComponent.
	 * @see OnStartProne
	 * @see IsProned
	 * @see CharacterMovement->WantsToProne
	 */
	UFUNCTION(BlueprintCallable, Category = Character, meta = (HidePin = "bClientSimulation"))
		virtual void Prone(bool bClientSimulation = false);

	/**
	 * Request the character to stop Proned. The request is processed on the next update of the CharacterMovementComponent.
	 * @see OnEndProne
	 * @see IsProned
	 * @see CharacterMovement->WantsToProne
	 */
	UFUNCTION(BlueprintCallable, Category = Character, meta = (HidePin = "bClientSimulation"))
		virtual void UnProne(bool bClientSimulation = false);

	/** @return true if this character is currently able to Prone (and is not currently Proned) */
	UFUNCTION(BlueprintCallable, Category = Character)
		virtual bool CanProne() const;

	/** Called when Character stops Proned. Called on non-owned Characters through bIsProned replication. */
	virtual void OnEndProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust);

	/** Event when Character stops Proned. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnEndProne", ScriptName = "OnEndProne"))
		void K2_OnEndProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust);

	/** Called when Character Pronees. Called on non-owned Characters through bIsProned replication. */
	virtual void OnStartProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust);

	/** Event when Character Pronees. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnStartProne", ScriptName = "OnStartProne"))
		void K2_OnStartProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust);
	
	UFUNCTION()
		virtual void OnRep_IsProned();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement) 
		USACharacterMovementComponent* SACharacterMovementComponent;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//	class UBoxComponent* ProneCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	virtual void BeginPlay() override;

	virtual void OnRep_AbilitySystemComponent() override;

private:

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Replicated)
		class USAInventoryComponent* InventoryComponent;	// 인벤토리 인터페이스나 Getter가 필요할 수도

	TScriptInterface<IInteractInterface> ThisInteract;
	TScriptInterface<IInteractInterface> LastInteract;
	bool isInteracting = false;
	float StandHeight;
	float StandRadius;

	float NowMaxSpeed;
	float NowMaxBackSpeed;

	UPROPERTY(EditDefaultsOnly)float MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly)float MaxWalkSpeed_Back;

	UPROPERTY(EditDefaultsOnly)float MaxSprintSpeed;
	UPROPERTY(EditDefaultsOnly)float MaxSprintSpeed_Back;

	UPROPERTY(EditDefaultsOnly)float MaxSneekSpeed;
	UPROPERTY(EditDefaultsOnly)float MaxSneekSpeed_Back;

	float PronedEyeHeight =30.f;

	void InitAbilityActorInfo();
	void BindEventCallback();
	void StanceEventCallback(const FGameplayEventData* Payload);

	void AbilityStart(const FGameplayTag& InputTag);
	void AbilityEnd(const FGameplayTag& InputTag);

	void CheckInteractInterface();

	bool IsPathClear(FVector Start, FVector End);
};
