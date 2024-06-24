// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "SA/Interface/AnimInterface.h"
#include "SACharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class SA_API ASACharacterBase : public ACharacter, public IAbilitySystemInterface, public IAnimInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASACharacterBase(const FObjectInitializer& ObjectInitializer);

	// Anim variable

	// Anim variable


	// Anim Interface
	virtual float GetSpeed() const override;
	virtual float GetDirection() const override;
	virtual float GetYaw() const override;
	virtual float GetPitch() const override;

	virtual bool IsInAir() const override;
	virtual bool IsAccelerating() const override;
	virtual bool IsAiming() const override;

	virtual ECharacterStance GetStance() const override;
	virtual EWeaponType GetEquippedWeaponType() const override;
	// Anim Interface

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
		TObjectPtr<UAttributeSet> AttributeSet;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	float Lean;
	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;

	UPROPERTY(BlueprintReadWrite, Category = Test, meta = (AllowPrivateAccess = "true")) // 나중에 bp에 노출 안할 거
	ECharacterStance CharacterStance = ECharacterStance::Stand;

	UPROPERTY(BlueprintReadWrite, Category = Test, meta = (AllowPrivateAccess = "true")) // 나중에 bp에 노출 안할 거
	EWeaponType EquippedWeaponType = EWeaponType::None;	 // 나중에 무기 변수로 바꿀 것, 무기타입은 무기->GetType할 것.

	UPROPERTY(BlueprintReadWrite, Category = Test, meta = (AllowPrivateAccess = "true")) // 나중에 지울 변수
		bool bIsAiming = false;

};
