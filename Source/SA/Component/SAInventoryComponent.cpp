// Fill out your copyright notice in the Description page of Project Settings.


#include "SAInventoryComponent.h"
#include "SA/SATagSingleton.h"
#include "SA/Interface/EquipmentInterface.h"
#include "SA/Item/SAWeaponBase.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Net/UnrealNetwork.h"

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

EWeaponType USAInventoryComponent::GetWeaponType()
{
	if (CurrentWeapon)
	{
		return CurrentWeapon->GetWeaponType();
	}
	return EWeaponType::None;
}

FTransform USAInventoryComponent::GetWeaponLeftHandSocketTransform() const
{
	if (!CurrentWeapon) return FTransform();
	if (!CurrentWeapon->GetWeaponMesh()) return FTransform();

	FTransform LeftHandSocketTransform = CurrentWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
	return LeftHandSocketTransform;
}



void USAInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAInventoryComponent, CurrentWeapon);
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
			//어떤 타입인지 체크 후 이미 장착한 상태면 장착한거 버리고 이걸로 낌.
			
			IEquipmentInterface* EquipmentIntf = const_cast<IEquipmentInterface*>(Cast<IEquipmentInterface>(Payload.OptionalObject));
			check(EquipmentIntf);	// Equip event는 EquipIntf 있는 얘만 보낼 수 있음
			EquipmentIntf->Equip(GetOwner());

			CurrentWeapon = const_cast<ASAWeaponBase*>(Cast<ASAWeaponBase>(Payload.OptionalObject)); // temp
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

