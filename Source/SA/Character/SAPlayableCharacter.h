// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SA/Character/SACharacterBase.h"
#include "SA/Interface/CommandInterface.h"
#include "GameplayTagContainer.h"
#include "SAPlayableCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SA_API ASAPlayableCharacter : public ASACharacterBase, public ICommandInterface
{
	GENERATED_BODY()
public:
	ASAPlayableCharacter(const FObjectInitializer& ObjectInitializer);

	// Command Interface
	virtual void MoveCommand(FVector2D Value) override;
	virtual void LookCommand(FVector2D Value) override;
	virtual void JumpCommand(bool Value) override;
	// command

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void Landed(const FHitResult& Hit) override;
protected:

private:	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	void InitAbilityActorInfo();
	void AbilityStart(const FGameplayTag& InputTag);
	void AbilityEnd(const FGameplayTag& InputTag);

};
