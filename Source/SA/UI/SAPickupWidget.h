// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SAPickupWidget.generated.h"

/**
 * 
 */
UCLASS()
class SA_API USAPickupWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemInfo)
		FText ItemName = NSLOCTEXT("Namespace", "Key", "DefaultName");

};
