// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SABlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SA_API USABlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Animation")
		static bool IsFullBodyMontagePlaying(UAnimInstance* AnimInstance);
};
