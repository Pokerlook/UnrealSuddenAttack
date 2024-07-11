// Fill out your copyright notice in the Description page of Project Settings.


#include "SABlueprintFunctionLibrary.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"


bool USABlueprintFunctionLibrary::IsFullBodyMontagePlaying(UAnimInstance* AnimInstance)
{
    if (AnimInstance)
    {
        UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
        if (CurrentMontage)
        {
            for (const FSlotAnimationTrack& SlotTrack : CurrentMontage->SlotAnimTracks)
            {
                if (SlotTrack.SlotName == FName("FullBody"))
                {
                    return true;
                }
            }
        }
    }
	return false;
}
