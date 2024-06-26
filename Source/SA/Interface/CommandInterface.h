// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputActionValue.h"
#include "CommandInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UCommandInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SA_API ICommandInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// BlueprintImplementableEvent, BlueprintNativeEvent
	UFUNCTION(BlueprintCallable, Category = "Command")
		virtual void MoveCommand(FVector2D Value) = 0;
	UFUNCTION(BlueprintCallable, Category = "Command")
		virtual void LookCommand(FVector2D Value) = 0;

	virtual void JumpCommand(bool Value) = 0;
};
