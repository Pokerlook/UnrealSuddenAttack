// Fill out your copyright notice in the Description page of Project Settings.


#include "SAItemBase.h"
#include "SA/UI/SAPickupWidget.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASAItemBase::ASAItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; 
	
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);

	ItemData.Name = NSLOCTEXT("Namespace", "Key", "DefaultItemName");
}

void ASAItemBase::ShowInteractWidget()
{
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(true);
	}
}

void ASAItemBase::HideInteractWidget()
{
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
}

void ASAItemBase::InteractStart()
{
	// Get Item, check get item event...
}

void ASAItemBase::InteractEnd()
{
	// Empty
}

void ASAItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASAItemBase, ItemState);
}

void ASAItemBase::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// add to inventory's overlapping items array
}

void ASAItemBase::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// remove from inventory's overlapping items array	
}

// Called when the game starts or when spawned
void ASAItemBase::BeginPlay()
{
	Super::BeginPlay();

	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
		if (USAPickupWidget* Widget = Cast< USAPickupWidget>(PickupWidget->GetWidget()))
		{
			Widget->ItemName = ItemData.Name;
		}
	}
}

void ASAItemBase::OnRep_ItemState()
{
	if (ItemState == EItemState::Dropped)
	{
		// Start the destruction timer when the item is dropped
//		GetWorldTimerManager().SetTimer(DestructionTimerHandle, this, &AItemActor::HandleItemDestruction, DestructionDelay, false);
	}
	else
	{
		// Clear the destruction timer if the state changes to anything else
//		GetWorldTimerManager().ClearTimer(DestructionTimerHandle);
	}
}
