// Fill out your copyright notice in the Description page of Project Settings.


#include "SAInventoryComponent.h"
#include "SA/SATagSingleton.h"
#include "SA/Interface/EquipmentInterface.h"
#include "SA/Item/SAWeaponBase.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

// Sets default values for this component's properties
USAInventoryComponent::USAInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

}

void USAInventoryComponent::InitInventory(UAbilitySystemComponent* ASC)
{
	const FSAGameplayTags& GameplayTags = FSAGameplayTags::Get();
	ASC->GenericGameplayEventCallbacks.FindOrAdd(GameplayTags.Event_Inventory_AddItem).AddUObject(this, &USAInventoryComponent::GameplayEventCallback);
	ASC->GenericGameplayEventCallbacks.FindOrAdd(GameplayTags.Event_Inventory_EquipItem).AddUObject(this, &USAInventoryComponent::GameplayEventCallback);
	ASC->GenericGameplayEventCallbacks.FindOrAdd(GameplayTags.Event_Inventory_DropItem).AddUObject(this, &USAInventoryComponent::GameplayEventCallback);
	ASC->GenericGameplayEventCallbacks.FindOrAdd(GameplayTags.Event_Inventory_UnequipItem).AddUObject(this, &USAInventoryComponent::GameplayEventCallback);
}

// Called when the game starts
void USAInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USAInventoryComponent::GameplayEventCallback(const FGameplayEventData* Payload)
{
	ENetRole NetRole = GetOwnerRole();

	if (NetRole == ROLE_Authority)
	{
		HandleGameplayEventInternal(*Payload);
	}
	else if (NetRole == ROLE_AutonomousProxy)
	{
		ServerHandleGameplayEvent(*Payload);
	}
}

void USAInventoryComponent::HandleGameplayEventInternal(FGameplayEventData Payload)
{
	ENetRole NetRole = GetOwnerRole();
	const FSAGameplayTags& GameplayTags = FSAGameplayTags::Get();

	if (NetRole == ROLE_Authority)
	{
		FGameplayTag EventTag = Payload.EventTag;

		if (EventTag == GameplayTags.Event_Inventory_AddItem)
		{
			// 인벤토리 구현할 때 제대로 추가.
			// 체크는 아이템쪽에서 interact했을 때하고, 여기서는 무조건 추가.	
		}
		else if (EventTag == GameplayTags.Event_Inventory_EquipItem)	// const_cast를 쓰지 않으면 복잡해질 거 같음.
		{
			//받은 Payload.OptionalObject; 캐스트해서(인터페이스?) 이게 무기인지 방어구인지 체크. 
			//어떤 타입인지 체크 후 이미 장착했으면 addItem, 아니면 계속 진행
			
			IEquipmentInterface* EquipmentIntf = const_cast<IEquipmentInterface*>(Cast<IEquipmentInterface>(Payload.OptionalObject));
			check(EquipmentIntf);	// Equip event는 EquipIntf 있는 얘만 보낼 수 있음
			EquipmentIntf->Equip(GetOwner());


			UE_LOG(LogTemp, Error, TEXT("Event_Inventory_EquipItem Tag get"));
		}
		else if (EventTag == GameplayTags.Event_Inventory_UnequipItem)
		{
			// 나중에, 언이큅 몽타주에서 이벤트 보낼것. 등에다가 무기 붙이기
		}
		else if (EventTag == GameplayTags.Event_Inventory_DropItem)
		{
			// 나중에 인벤토리 구현할 때 .
		}
	}
}

void USAInventoryComponent::ServerHandleGameplayEvent_Implementation(FGameplayEventData Payload)
{
	HandleGameplayEventInternal(Payload);
}

