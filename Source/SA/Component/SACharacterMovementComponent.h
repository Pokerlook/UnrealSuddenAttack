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
	UPROPERTY(EditDefaultsOnly) float SlideGravityForce = 200;
	UPROPERTY(EditDefaultsOnly) float Slide_Friction = .2;
	UPROPERTY(EditDefaultsOnly) float BrakingDecelerationSliding = 1000.f;

	// Transient
	UPROPERTY(Transient) class ASAPlayableCharacter*SACharacterOwner;
//	bool Safe_bPrevWantsToCrouch;
	bool Safe_bWantsToSlide;


public:
	USACharacterMovementComponent();

	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;

	UFUNCTION(BlueprintPure) bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;

	void Slide();
	void UnSlide();

protected:
	virtual void InitializeComponent() override;

	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

private:
	void Safe_EnterSlide();
	void Safe_ExitSlide();
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;
	bool CanSlide() const;

};
