// Fill out your copyright notice in the Description page of Project Settings.


#include "SA/SAAssetManager.h"
#include "SA/SATagSingleton.h"
#include "AbilitySystemGlobals.h"

USAAssetManager& USAAssetManager::Get()
{
	check(GEngine);

	USAAssetManager* SAAssetManager = Cast<USAAssetManager>(GEngine->AssetManager);
	return *SAAssetManager;
}

void USAAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FSAGameplayTags::InitializeNativeGameplayTags();

	// This is required to use Target Data!
	UAbilitySystemGlobals::Get().InitGlobalData();
}
