// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SA/Interface/InteractInterface.h"
#include "SA/SATypes.h"
#include "SAItemBase.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FS_ItemStaticData
{
	GENERATED_USTRUCT_BODY()

public:
	FS_ItemStaticData() : IdentifierTag(), Name(), ItemExplanation(), Icon(), RefClass(), StackSize(1)
	{}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FGameplayTag IdentifierTag;	// 이걸로 데이터베이스에서 서치.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText ItemExplanation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UTexture2D* Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<class ASAItemBase> RefClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int StackSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Quantity;
};

UCLASS()
class SA_API ASAItemBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASAItemBase();
	
	// interact intf
	virtual void ShowInteractWidget() override;
	virtual void HideInteractWidget() override;
	virtual void InteractStart(AActor* Interactor) override;
	virtual void InteractEnd(AActor* Interactor) override;
	// interact intf


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
		void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
		void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Item")
		class UWidgetComponent* PickupWidget;
	UPROPERTY(VisibleAnywhere, Category = "Item")// Replicated 해야하나?
		FS_ItemStaticData ItemData;

	// 이름(태그), 설명, 아이템 클래스 는 Static Data 구조체 -> 런타임에 안 바뀜
	// 그 외에는 상속받은 곳에서...
private:
	USceneComponent* RootSceneComponent;


	UPROPERTY(ReplicatedUsing = OnRep_ItemState)
		EItemState ItemState = EItemState::Initial;

	UFUNCTION()
		void OnRep_ItemState();

	//// Destruction timer handle
	//FTimerHandle DestructionTimerHandle;
	//// Destruction delay in seconds
	//UPROPERTY(EditAnywhere, Category = "Item Destory Parameter")
	//	float DestructionDelay = 10.0f; // Adjust as necessary
};
