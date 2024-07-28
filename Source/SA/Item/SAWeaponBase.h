// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAEquipmentItem.h"
#include "SA/SATypes.h"
#include "SAWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class SA_API ASAWeaponBase : public ASAEquipmentItem
{
	GENERATED_BODY()

public:
	ASAWeaponBase();

	//equip
	virtual void Equip(AActor* InOwner) override;
	//equip

	void UpdateWeaponSocket(bool isProne);

	EWeaponType GetWeaponType();

	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)	// 이것도 캐릭터ASC 태그 콜백으로 prone add나 remove면 attach 바꿔야 할 것
		FName AttachmentSocket = FName("Socket_Gun");
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName AttachmentSocketProne = FName("Socket_GunProne");	// 아니면 그냥 prone일떄랑 아닐때 weapon 회전?


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EWeaponType WeaponType = EWeaponType::MainRangeWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float FireRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float BaseDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USoundBase* AttackSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FGameplayTag AmmoTag;

	// weapon projectile class
	
	virtual void OnRep_ItemState() override;
private:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
		USkeletalMeshComponent* WeaponMesh;	// has LeftHandSocket.

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		class USphereComponent* AreaSphere;


};
