// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SAAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class SA_API USAAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static USAAssetManager& Get();
protected:

	virtual void StartInitialLoading() override;
};
