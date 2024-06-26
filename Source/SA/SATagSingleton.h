// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
//#include "SATagSingleton.generated.h"

struct FSAGameplayTags
{
public:
	static const FSAGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();	// in AssetManager

	/*
		InputTags
	*/
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look;
	FGameplayTag InputTag_Jump;
	FGameplayTag InputTag_Crouch;	// c
	FGameplayTag InputTag_Prone;	// z
	FGameplayTag InputTag_Sneek;	// left ctrl
	FGameplayTag InputTag_Sprint;	// left shift
	FGameplayTag InputTag_Fire;
	FGameplayTag InputTag_Aim;
	FGameplayTag InputTag_Reload;	// r
	FGameplayTag InputTag_HolsterWeapon;	// x
	FGameplayTag InputTag_Weapon1;
	FGameplayTag InputTag_Weapon2;
	FGameplayTag InputTag_Weapon3;
	FGameplayTag InputTag_Weapon4;
	FGameplayTag InputTag_Weapon5;
	FGameplayTag InputTag_LeanLeft;		// q
	FGameplayTag InputTag_LeanRight;	// e
	FGameplayTag InputTag_Interact;		// f
	FGameplayTag InputTag_ShootMode;	//b
	FGameplayTag InputTag_ViewMode;	// v


//	FGameplayTag InputTag_Inventory;	// I
//	FGameplayTag InputTag_Map;	// m
	FGameplayTag InputTag_MiniMap;	// n
	FGameplayTag InputTag_ESC;
	FGameplayTag InputTag_Leaderboard;

private:
	static FSAGameplayTags GameplayTags;
};