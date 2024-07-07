// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAItemBase.h"
#include "SA/Interface/EquipmentInterface.h"
#include "SAEquipmentItem.generated.h"

struct FGameplayAbilitySpecHandle;
struct FActiveGameplayEffectHandle;

/**
 * 
 */
UCLASS()
class SA_API ASAEquipmentItem : public ASAItemBase, public IEquipmentInterface
{
	GENERATED_BODY()
public:

	virtual void Equip(AActor* InOwner) override;
	virtual void Unequip(AActor* InOwner) override;

	virtual void OnEquip() override;
	virtual void OnUnequip() override;

	//interact
	virtual void InteractStart(AActor* Interactor) override;
	virtual void InteractEnd(AActor* Interactor) override;
	//interact

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Eqeuipment")
		TArray<TSubclassOf<class UGameplayEffect>> OngoingEffects;	// at equip add, at uneqeuip remove
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Eqeuipment")
		TArray<TSubclassOf<class UGameplayAbility>> GrantedAbilities; // at equip add, at uneqeuip remove

	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;
	TArray<FActiveGameplayEffectHandle> OngoingEffectHandles;


	void TryGrantAbilities(AActor* InOwner);
	void TryRemoveAbilities(AActor* InOwner);
	void TryApplyEffects(AActor* InOwner);
	void TryRemoveEffects(AActor* InOwner);
};
