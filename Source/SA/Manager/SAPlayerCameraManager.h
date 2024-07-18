// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "SAPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class SA_API ASAPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	ASAPlayerCameraManager();
	

	UPROPERTY(EditDefaultsOnly) float CrouchBlendDuration = .5f;
	float CrouchBlendTime;


	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

};
