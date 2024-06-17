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
	ASACharacterBase();

	// Anim Interface
	virtual float GetSpeed() const override;

	virtual bool IsInAir() const override;
	virtual bool IsAccelerating() const override;
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

};
