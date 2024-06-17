// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SA/Character/SACharacterBase.h"
#include "SA/Interface/CommandInterface.h"
#include "SAPlayableCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SA_API ASAPlayableCharacter : public ASACharacterBase, public ICommandInterface
{
	GENERATED_BODY()
public:
	ASAPlayableCharacter();

	// Command Interface
	virtual void Move(FVector2D Value) override;
	virtual void Look(FVector2D Value) override;
	// command

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:

private:
	void InitAbilityActorInfo();

};
