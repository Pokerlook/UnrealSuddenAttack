// Fill out your copyright notice in the Description page of Project Settings.


#include "SAItemBase.h"
#include "Components/WidgetComponent.h"
#include "SA/UI/SAPickupWidget.h"

// Sets default values
ASAItemBase::ASAItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
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

void ASAItemBase::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ShowInteractWidget();	// Test. It will be at player controller's line trace.
}

void ASAItemBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	HideInteractWidget();
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
