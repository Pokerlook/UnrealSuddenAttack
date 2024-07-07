// Fill out your copyright notice in the Description page of Project Settings.


#include "SAWeaponBase.h"
#include "Components/SphereComponent.h"
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

void ASAWeaponBase::Equip(AActor* InOwner)
{
	check(InOwner);
	SetOwner(InOwner);
	ACharacter* Character = Cast<ACharacter>(InOwner);
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(AttachmentSocket);
	check(HandSocket);
	this->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), AttachmentSocket); // scale1로 하면 뭔가 어색
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HideInteractWidget();
}
