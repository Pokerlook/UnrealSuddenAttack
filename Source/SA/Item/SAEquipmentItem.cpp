// Fill out your copyright notice in the Description page of Project Settings.


#include "SAEquipmentItem.h"
#include "SA/SATagSingleton.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "GameplayAbilitySpec.h"

void ASAEquipmentItem::Equip(AActor* InOwner)
{
	// on child
}

void ASAEquipmentItem::Unequip(AActor* InOwner)
{
	// on child
}

void ASAEquipmentItem::OnEquip()
{
	TryGrantAbilities(GetOwner());
	TryApplyEffects(GetOwner());
}

void ASAEquipmentItem::OnUnequip()
{
	TryRemoveAbilities(GetOwner());
	TryRemoveEffects(GetOwner());
}

void ASAEquipmentItem::InteractStart(AActor* Interactor)
{	// check if interactor has equipping weapon, this weapon's type. then equip.
	// if already has. call super
	// if not. send gameplay event to actor, equip item event.
	// 아니면. 그냥 inventory 콜백함수에서 처리할까.


	const FSAGameplayTags& GameplayTags = FSAGameplayTags::Get();

	FGameplayEventData EventPayload;
	EventPayload.EventTag = GameplayTags.Event_Inventory_EquipItem;
	EventPayload.OptionalObject = this;
	EventPayload.Instigator = this;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Interactor, GameplayTags.Event_Inventory_EquipItem, EventPayload);
}

void ASAEquipmentItem::InteractEnd(AActor* Interactor)
{
}

void ASAEquipmentItem::TryGrantAbilities(AActor* InOwner)
{
	if (InOwner && InOwner->HasAuthority())
	{
		if (UAbilitySystemComponent* AbilityComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
		{
			for (auto ItemAbility : GrantedAbilities)
			{
				GrantedAbilityHandles.Add(AbilityComponent->GiveAbility(FGameplayAbilitySpec(ItemAbility)));
			}
		}
	}
}

void ASAEquipmentItem::TryRemoveAbilities(AActor* InOwner)
{
	if (InOwner && InOwner->HasAuthority())
	{
		if (UAbilitySystemComponent* AbilityComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
		{
			for (auto AbilityHandle : GrantedAbilityHandles)
			{
				AbilityComponent->ClearAbility(AbilityHandle);
			}

			GrantedAbilityHandles.Empty();
		}
	}
}

void ASAEquipmentItem::TryApplyEffects(AActor* InOwner)
{
	if (UAbilitySystemComponent* AbilityComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
	{
		FGameplayEffectContextHandle EffectContext = AbilityComponent->MakeEffectContext();

		for (auto GameplayEffect : OngoingEffects)
		{
			if (!GameplayEffect.Get()) continue;

			FGameplayEffectSpecHandle SpecHandle = AbilityComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
			if (SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilityComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				if (!ActiveGEHandle.WasSuccessfullyApplied())
				{
					ABILITY_LOG(Log, TEXT("Item %s failed to apply runtime effect %s"), *GetName(), *GetNameSafe(GameplayEffect));
				}
				else
				{
					OngoingEffectHandles.Add(ActiveGEHandle);
				}
			}
		}
	}
}

void ASAEquipmentItem::TryRemoveEffects(AActor* InOwner)
{
	if (UAbilitySystemComponent* AbilityComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
	{
		for (FActiveGameplayEffectHandle ActiveEffectHandle : OngoingEffectHandles)
		{
			if (ActiveEffectHandle.IsValid())
			{
				AbilityComponent->RemoveActiveGameplayEffect(ActiveEffectHandle);
			}
		}
	}

	OngoingEffectHandles.Empty();
}
