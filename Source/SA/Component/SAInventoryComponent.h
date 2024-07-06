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
	virtual void InitializeComponent() override;


protected:
	virtual void BeginPlay() override;

private:
	class ASAWeaponBase* CurrentWeapon = nullptr; // now weapon in hands
	class ASAWeaponBase* NextWeapon = nullptr; // next weapon to change
//	TArray<ASAWeaponBase> EquippingWeapons;
	// equipment(Çï¸ä,Á¶³¢,¹è³¶?) ¹è¿­ º¯¼ö ³ªÁß¿¡ Ãß°¡ÇÒ µí. mapÀ¸·Î?


	virtual void GameplayEventCallback(const FGameplayEventData* Payload);

	void HandleGameplayEventInternal(FGameplayEventData Payload);
	UFUNCTION(Server, Reliable)
		void ServerHandleGameplayEvent(FGameplayEventData Payload);

};
