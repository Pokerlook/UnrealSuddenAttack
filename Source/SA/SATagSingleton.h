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
	FGameplayTag InputTag_Move;		// wsad
	FGameplayTag InputTag_Look;
	FGameplayTag InputTag_Jump;		// space
	FGameplayTag InputTag_Crouch;	// c
	FGameplayTag InputTag_Prone;	// z
	FGameplayTag InputTag_Sneek;	// left ctrl
	FGameplayTag InputTag_Sprint;	// left shift
	FGameplayTag InputTag_Fire;		// lmb
	FGameplayTag InputTag_Aim;		// rmb
	FGameplayTag InputTag_Wheel;
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
	FGameplayTag InputTag_DropWeapon; // G
	FGameplayTag InputTag_Vault;	// h
	FGameplayTag InputTag_FreeLook;	// left alt


	FGameplayTag InputTag_Inventory;	// I
	FGameplayTag InputTag_Map;	// m
	FGameplayTag InputTag_MiniMap;	// n
	FGameplayTag InputTag_ESC;
	FGameplayTag InputTag_Leaderboard;	// tab
	FGameplayTag InputTag_WeaponMenu;	// p
	/*
		InputTags
	*/


	/*
		StateTags
	*/
	FGameplayTag State_InAir_Jumping;
	FGameplayTag State_Stance_Crouch;
	FGameplayTag State_Stance_Prone;
	FGameplayTag State_Movement_Sprint;
	FGameplayTag State_Movement_Sneek;
	/*
		StateTags
	*/


	/*
		LocomotionTags
	*/
	FGameplayTag Ability_Locomotion_Jump;
	FGameplayTag Ability_Locomotion_Crouch;
	FGameplayTag Ability_Locomotion_Prone;
	FGameplayTag Ability_Locomotion_Sprint;
	FGameplayTag Ability_Locomotion_Sneek;
	/*
		LocomotionTags
	*/


	/*
		ItemTags
	*/
	FGameplayTag Item_Weapon_Gun_AssaultRifle;

	FGameplayTag Item_Equipment_;

	FGameplayTag Item_Fashion_;
	/*
		ItemTags
	*/

	/*
		EventTags
	*/
	FGameplayTag Event_Inventory_AddItem;
	FGameplayTag Event_Inventory_EquipItem;
	FGameplayTag Event_Inventory_DropItem;
	FGameplayTag Event_Inventory_UnequipItem;
	FGameplayTag Event_Locomotion_Stand;
	FGameplayTag Event_Locomotion_Crouch;
	FGameplayTag Event_Locomotion_Prone;
	/*
		EventTags
	*/

private:
	static FSAGameplayTags GameplayTags;
};