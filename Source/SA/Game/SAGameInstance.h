// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SA/SATypes.h"
#include "SAGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SA_API USAGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UDataTable* ItemDT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGameMode SelectedGameMode = EGameMode::FreeForAll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGameMap SelectedGameMap = EGameMap::Map1;
};
