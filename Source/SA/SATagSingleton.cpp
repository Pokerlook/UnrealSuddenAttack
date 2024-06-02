// Fill out your copyright notice in the Description page of Project Settings.


#include "SA/SATagSingleton.h"
#include "GameplayTagsManager.h"

FSAGameplayTags FSAGameplayTags::GameplayTags;

void FSAGameplayTags::InitializeNativeGameplayTags()
{

	/// Input
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
	GameplayTags.InputTag_Crouch = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Crouch"),
		FString("Input Tag for Crouch, Default is C")
	);	
	GameplayTags.InputTag_Prone = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Prone"),
		FString("Input Tag for Prone, Default is Z")
	);	
	GameplayTags.InputTag_Sneek = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Sneek"),
		FString("Input Tag for Sneek, Default is Left Ctrl")
	);
	GameplayTags.InputTag_Sprint = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Sprint"),
		FString("Input Tag for Sprint, Default is Left Shift")
	);
	GameplayTags.InputTag_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Fire"),
		FString("Input Tag for Fire, Default is Left Mouse Button")
	);
	GameplayTags.InputTag_Aim = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Aim"),
		FString("Input Tag for Aim, Default is Right Mouse Button")
	);
	GameplayTags.InputTag_Wheel = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Wheel"),
		FString("Input Tag for Wheel, Default is Mouse Wheel ")
	);
	GameplayTags.InputTag_Reload = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Reload"),
		FString("Input Tag for Reload, Default is R")
	);
	GameplayTags.InputTag_HolsterWeapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.HolsterWeapon"),
		FString("Input Tag for HolsterWeapon, Default is X")
	);
	GameplayTags.InputTag_Weapon1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Weapon1"),
		FString("Input Tag for Weapon1, Default is 1")
	);
	GameplayTags.InputTag_Weapon2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Weapon2"),
		FString("Input Tag for Weapon2, Default is 2")
	);
	GameplayTags.InputTag_Weapon3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Weapon3"),
		FString("Input Tag for Weapon3, Default is 3")
	);
	GameplayTags.InputTag_Weapon4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Weapon4"),
		FString("Input Tag for Weapon4, Default is 4")
	);
	GameplayTags.InputTag_Weapon5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Weapon5"),
		FString("Input Tag for Weapon5, Default is 5")
	);
	GameplayTags.InputTag_LeanLeft = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LeanLeft"),
		FString("Input Tag for LeanLeft, Default is Q")
	);
	GameplayTags.InputTag_LeanRight = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LeanRight"),
		FString("Input Tag for LeanRight, Default is E")
	);
	GameplayTags.InputTag_Interact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Interact"),
		FString("Input Tag for Interact, Default is F")
	);
	GameplayTags.InputTag_ShootMode = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.ShootMode"),
		FString("Input Tag for ShootMode, Default is B")
	);
	GameplayTags.InputTag_ViewMode = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.ViewMode"),
		FString("Input Tag for ViewMode, Default is V")
	);
	GameplayTags.InputTag_DropWeapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.DropWeapon"),
		FString("Input Tag for DropWeapon, Default is G")
	);
	GameplayTags.InputTag_Vault = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Vault"),
		FString("Input Tag for Vault, Default is H")
	);
	GameplayTags.InputTag_FreeLook = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.FreeLook"),
		FString("Input Tag for FreeLook, Default is Left Alt")
	);

	//

	GameplayTags.InputTag_Inventory = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Inventory"),
		FString("Input Tag for Inventory, Default is I")
	);
	GameplayTags.InputTag_Map = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Map"),
		FString("Input Tag for Map, Default is M")
	);
	GameplayTags.InputTag_MiniMap = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.MiniMap"),
		FString("Input Tag for MiniMap, Default is N")
	);
	GameplayTags.InputTag_ESC = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.ESC"),
		FString("Input Tag for ESC, Default is ESC")
	);
	GameplayTags.InputTag_Leaderboard = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Leaderboard"),
		FString("Input Tag for Leaderboard, Default is TAB")
	);
	GameplayTags.InputTag_WeaponMenu = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.WeaponMenu"),
		FString("Input Tag for WeaponMenu, Default is P")
	);
	// Input


}
