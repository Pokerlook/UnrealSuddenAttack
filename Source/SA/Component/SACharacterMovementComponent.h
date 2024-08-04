// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SACharacterMovementComponent.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_Slide			UMETA(DisplayName = "Slide"),
	CMOVE_MAX			UMETA(Hidden),
};

UCLASS()
class SA_API USACharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_SA : public FSavedMove_Character // UObject를 쓰지 않는 이유, 작게 유지하기 위함
	{
		typedef FSavedMove_Character Super;

	public:
		enum CompressedFlags
		{
			FLAG_Prone = 0x10,
			FLAG_Custom_1 = 0x20,
			FLAG_Custom_2 = 0x40,
			FLAG_Custom_3 = 0x80,
		};

		// Flags, rep
		// uint8 Saved_bWantsToSprint:1; but I will sprint with GAS

		// Other Variables, not rep
		uint8 Saved_bWantsToSlide : 1;
		uint32 bWantsToProne : 1;
		uint32 bProneLocked : 1;

		FSavedMove_SA();

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	// custom FSavedMove_Character사용하기 위함
	class FNetworkPredictionData_Client_SA : public FNetworkPredictionData_Client_Character
	{
		typedef FNetworkPredictionData_Client_Character Super;
	public:
		FNetworkPredictionData_Client_SA(const UCharacterMovementComponent& ClientMovement);
		virtual FSavedMovePtr AllocateNewMove() override;
	};

	// Parameters
	UPROPERTY(EditDefaultsOnly) float MinSlideSpeed = 400;
	UPROPERTY(EditDefaultsOnly) float MaxSlideSpeed = 400.f;
	UPROPERTY(EditDefaultsOnly) float SlideEnterImpulse = 400;
	UPROPERTY(EditDefaultsOnly) float SlideGravityForce = 4000;
	UPROPERTY(EditDefaultsOnly) float SlideFrictionFactor = 0.06f;
	UPROPERTY(EditDefaultsOnly) float BrakingDecelerationSliding = 1000.f;

	// Transient
	UPROPERTY(Transient) class ASAPlayableCharacter*SACharacterOwner;
//	bool Safe_bPrevWantsToCrouch;
	bool Safe_bWantsToSlide;


public:
	USACharacterMovementComponent();

	/** Max Acceleration (rate of change of velocity) */
	UPROPERTY(Category = "Character Movement (General Settings)", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float MaxAccelerationProned = 256.f;
	/**
	 * Deceleration when walking and not applying acceleration. This is a constant opposing force that directly lowers velocity by a constant value.
	 * @see GroundFriction, MaxAcceleration
	 */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float BrakingDecelerationProned = 512.f;
	/**
	 * Setting that affects movement control. Higher values allow faster changes in direction.
	 * If bUseSeparateBrakingFriction is false, also affects the ability to stop more quickly when braking (whenever Acceleration is zero), where it is multiplied by BrakingFrictionFactor.
	 * When braking, this property allows you to control how much friction is applied when moving across the ground, applying an opposing force that scales with current velocity.
	 * This can be used to simulate slippery surfaces such as ice or oil by changing the value (possibly based on the material pawn is standing on).
	 * @see BrakingDecelerationWalking, BrakingFriction, bUseSeparateBrakingFriction, BrakingFrictionFactor
	 */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float GroundFrictionProned = 3.f;
	/**
	 * Friction (drag) coefficient applied when braking (whenever Acceleration = 0, or if character is exceeding max speed); actual value used is this multiplied by BrakingFrictionFactor.
	 * When braking, this property allows you to control how much friction is applied when moving across the ground, applying an opposing force that scales with current velocity.
	 * Braking is composed of friction (velocity-dependent drag) and constant deceleration.
	 * This is the current value, used in all movement modes; if this is not desired, override it or bUseSeparateBrakingFriction when movement mode changes.
	 * @note Only used if bUseSeparateBrakingFriction setting is true, otherwise current friction such as GroundFriction is used.
	 * @see bUseSeparateBrakingFriction, BrakingFrictionFactor, GroundFriction, BrakingDecelerationWalking
	 */
	UPROPERTY(Category = "Character Movement (General Settings)", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", EditCondition = "bUseSeparateBrakingFriction"))
		float BrakingFrictionProned = 1.f;
	/** Collision half-height when proned (component scale is applied separately) */
	UPROPERTY(Category = "Character Movement (General Settings)", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ForceUnits = cm))
		float PronedHalfHeight = 25.f;
	/** Collision half-height when proned (component scale is applied separately) */
	UPROPERTY(Category = "Character Movement (General Settings)", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ForceUnits = cm))
		float PronedRadius = 20.f;

	/**
	 * Cannot leave prone for this duration
	 * @see ClearProneLock
	 */
	UPROPERTY(Category = "Character Movement (General Settings)", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ForceUnits = cm))
		float ProneLockDuration = 1.f;
	/** If true, Character can walk off a ledge when proned. */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite)
		uint8 bCanWalkOffLedgesWhenProned : 1;
	/** If true, try to Prone (or keep Proned) on next update. If false, try to stop Proned on next update. */
	UPROPERTY(Category = "Character Movement (General Settings)", VisibleInstanceOnly, BlueprintReadOnly)
		uint8 bWantsToProne : 1;

	UPROPERTY(Category = "Character Movement (General Settings)", VisibleInstanceOnly, BlueprintReadOnly)
		uint8 bProneLocked : 1;

	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	virtual bool HasValidData() const override;
	virtual void PostLoad() override;
	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;
	virtual float GetMaxAcceleration() const override;
	virtual float GetMaxBrakingDeceleration() const override;
	virtual void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;
	virtual void ApplyVelocityBraking(float DeltaTime, float Friction, float BrakingDeceleration) override;

	virtual bool CanWalkOffLedges() const override;
	virtual bool CanAttemptJump() const override;
	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;

	UFUNCTION(BlueprintPure) bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;

	void Slide();
	void UnSlide();


	UFUNCTION(BlueprintPure, Category = "Character Movement")
		bool IsProneLocked() const;
	bool IsProneLockOnTimer() const;
	float GetRemainingProneLockCooldown() const;	// Prone lock timer
	void SetProneLock(bool bLock);
	float GetTimestamp() const;

	virtual bool IsProned() const;
	/**
	 * Call CharacterOwner->OnStartProne() if successful.
	 * In general you should set bWantsToProne instead to have the Prone persist during movement, or just use the Prone functions on the owning Character.
	 * @param	bClientSimulation	true when called when bIsProned is replicated to non owned clients.
	 */
	virtual void Prone(bool bClientSimulation = false);

	/**
	 * Checks if default capsule size fits (no encroachment), and trigger OnEndProne() on the owner if successful.
	 * @param	bClientSimulation	true when called when bIsProned is replicated to non owned clients.
	 */
	virtual void UnProne(bool bClientSimulation = false);

	/** Returns true if the character is allowed to Prone in the current state. By default it is allowed when walking or falling. */
	virtual bool CanProneInCurrentState() const;

//	virtual void PhysicsRotation(float DeltaTime) override; 
//	virtual FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime, FRotator& DeltaRotation) const override;
//	virtual void UpdateBasedRotation(FRotator& FinalRotation, const FRotator& ReducedRotation) override;
	virtual void UpdateBasedMovement(float DeltaSeconds);
protected:
	float ProneLockTimestamp = -1.f;

	virtual void InitializeComponent() override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual bool ClientUpdatePositionAfterServerUpdate() override;

	virtual bool MoveUpdatedComponentImpl(const FVector& Delta, const FQuat& NewRotation, bool bSweep, FHitResult* OutHit = NULL, ETeleportType Teleport = ETeleportType::None) override;
	virtual void PhysWalking(float deltaTime, int32 Iterations) override;
private:
	void Safe_EnterSlide(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void Safe_ExitSlide();
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;
	bool CanSlide() const;
	bool ProneSweepMove(const FVector& DeltaLocation, const FQuat& DeltaRotation, FHitResult* OutHit);
	bool TryProneMove(const FVector& DeltaLocation, const FQuat& DeltaRotation, const bool bSweep, const ETeleportType Teleport, FHitResult* OutHit);
};
