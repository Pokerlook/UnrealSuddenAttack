// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "SAInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SA_API USAInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USAInventoryComponent();
	void InitInventory(UAbilitySystemComponent* ASC);

//	void ChangeWeaponWithItem(ASAWeaponBase* ToChangeWeapon); // inventory ui만들 때 필요
//인벤토리에서 장착하면, 이벤트 받았을때랑 달리 이미 장착한 거라도 강제로 바꿈. 현재 unequip&destory. 이거 spawn&eqeuip

protected:
	virtual void BeginPlay() override;

private:
	class ASAWeaponBase* CurrentWeapon = nullptr; // now weapon in hands
	class ASAWeaponBase* NextWeapon = nullptr; // next weapon to change
//	TArray<ASAWeaponBase> EquippingWeapons;
	// equipment(헬멧,조끼,배낭?) 배열 변수 나중에 추가할 듯. map으로?


	virtual void GameplayEventCallback(const FGameplayEventData* Payload);

	void HandleGameplayEventInternal(FGameplayEventData Payload);
	UFUNCTION(Server, Reliable)
		void ServerHandleGameplayEvent(FGameplayEventData Payload);

};
