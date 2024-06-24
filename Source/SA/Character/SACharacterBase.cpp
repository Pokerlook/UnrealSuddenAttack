// Fill out your copyright notice in the Description page of Project Settings.


#include "SA/Character/SACharacterBase.h"
#include "SA/AbilitySystem/SAAbilitySystemComponent.h"
#include "SA/AbilitySystem/SAAttributeSet.h"
#include "SA/Component/SACharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASACharacterBase::ASACharacterBase(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer.SetDefaultSubobjectClass<USACharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

float ASACharacterBase::GetSpeed() const
{
	
	return GetVelocity().Length();
}

float ASACharacterBase::GetDirection() const
{    
    if (!GetVelocity().IsNearlyZero())
    {
        FMatrix RotMatrix = FRotationMatrix(GetActorRotation());
        FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
        FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
        FVector NormalizedVel = GetVelocity().GetSafeNormal2D();

        // get a cos(alpha) of forward vector vs velocity
        float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
        // now get the alpha and convert to degree
        float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

        // depending on where right vector is, flip it
        float RightCosAngle = FVector::DotProduct(RightVector, NormalizedVel);
        if (RightCosAngle < 0)
        {
            ForwardDeltaDegree *= -1;
        }

        return ForwardDeltaDegree;
    }
    return 0.f;
}

float ASACharacterBase::GetYaw() const
{
    return UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation()).Yaw;
}

float ASACharacterBase::GetPitch() const
{
    float Pitch; 
    Pitch = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation()).Pitch;
    if (Pitch > 90.f && !IsLocallyControlled())
    {
        // map pitch from [270, 360) to [-90, 0)
        FVector2D InRange(270.f, 360.f);
        FVector2D OutRange(-90.f, 0.f);
        Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, Pitch);
    }
	return Pitch;
}

bool ASACharacterBase::IsInAir() const
{
    return GetCharacterMovement()->IsFalling();
}

bool ASACharacterBase::IsAccelerating() const
{
    if (GetCharacterMovement()->GetCurrentAcceleration().Size() > 0)
        return true;
	return false;
}

bool ASACharacterBase::IsAiming() const
{
    // ���߿� Aiming ���� �±� üũ �� ����
    return bIsAiming;
}

ECharacterStance ASACharacterBase::GetStance() const
{
    return CharacterStance;
}

EWeaponType ASACharacterBase::GetEquippedWeaponType() const
{
    return EquippedWeaponType;
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

    CharacterRotationLastFrame = CharacterRotation;
    CharacterRotation = GetActorRotation();
    const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
    const float Target = Delta.Yaw / DeltaTime;
    const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
    Lean = FMath::Clamp(Interp, -45.f, 45.f);

    // ���̷�Ż �޽� spine1? Lean��ŭ ����̱�

}

// Called to bind functionality to input
void ASACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ASACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

