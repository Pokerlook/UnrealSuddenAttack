// Fill out your copyright notice in the Description page of Project Settings.


#include "SAWeaponBase.h"
#include "Components/SphereComponent.h"
#include "SA/SATagSingleton.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "Engine/SkeletalMeshSocket.h"


ASAWeaponBase::ASAWeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
}

void ASAWeaponBase::InteractStart(AActor* Interactor)
{
	// check if interactor has equipping weapon, this weapon's type. then equip.
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

void ASAWeaponBase::InteractEnd(AActor* Interactor)
{
}

void ASAWeaponBase::Equip(AActor* InOwner)
{
	check(InOwner);
	SetOwner(InOwner);
	ACharacter* Character = Cast<ACharacter>(InOwner);
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(AttachmentSocket);
	check(HandSocket);
	this->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), AttachmentSocket); // scale1로 하면 뭔가 어색
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
