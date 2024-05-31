// Fill out your copyright notice in the Description page of Project Settings.


#include "SA/SATagSingleton.h"
#include "GameplayTagsManager.h"

FSAGameplayTags FSAGameplayTags::GameplayTags;

void FSAGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.InputTag_Move = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Move"),
		FString("Input Tag for Move, Default is WSAD")
	);	
	GameplayTags.InputTag_Look = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Look"),
		FString("Input Tag for Look, Default is Mouse XY")
	);
	GameplayTags.InputTag_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Jump"),
		FString("Input Tag for Jump, Default is Space")
	);

	GameplayTags.InputTag_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Fire"),
		FString("Input Tag for Fire, Default is Left Mouse Button")
	);
	GameplayTags.InputTag_Aim = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Aim"),
		FString("Input Tag for Aim, Default is Right Mouse Button")
	);

}
