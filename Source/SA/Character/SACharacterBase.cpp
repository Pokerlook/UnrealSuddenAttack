// Fill out your copyright notice in the Description page of Project Settings.


#include "SA/Character/SACharacterBase.h"

// Sets default values
ASACharacterBase::ASACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

