// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SA/Interface/InteractInterface.h"
#include "SA/SATypes.h"
#include "SAItemBase.generated.h"

UCLASS()
class SA_API ASAItemBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASAItemBase();
	
	virtual void ShowInteractWidget() override;
	virtual void HideInteractWidget() override;

	UFUNCTION(BlueprintCallable)
		void OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
		void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// 이름(태그), 설명, 아이템 클래스 는 Static Data 구조체 -> 런타임에 안 바뀜
	// 그 외에는 상속받은 곳에서...
private:
	UPROPERTY(VisibleAnywhere, Category = "Item")
		class UWidgetComponent* PickupWidget;
	UPROPERTY(VisibleAnywhere, Category = "Item")
		FS_ItemStaticData ItemData;
};
