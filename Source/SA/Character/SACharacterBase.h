// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "SA/Interface/AnimInterface.h"
#include "SACharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class SA_API ASACharacterBase : public ACharacter, public IAbilitySystemInterface, public IAnimInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASACharacterBase(const FObjectInitializer& ObjectInitializer);

	// Anim Interface
	virtual float GetSpeed() const override;
	virtual float GetDirection() const override;
	virtual float GetYaw() const override;
	virtual float GetPitch() const override;

	virtual bool IsInAir() const override;
	virtual bool IsAccelerating() const override;
	virtual bool IsAiming() const override;		// aiming은 기본적으로 true로 하고. 총 안쏘고 3초?5초? 후에 false로 바꾸는데, 태그에 aiming 이면 안 바꿈...
	// 태그 추가&제거마다 타이머 클리어하고 다시 돌리고 하면 되겠다. // aiming이면 컨트롤러쪽으로 캐릭터 회전.
	virtual bool ShouldUseIK() const override;
	virtual bool IsAnyMontagePlaying() const override;

	virtual ECharacterStance GetStance() const override;
	virtual EWeaponType GetEquippedWeaponType() const override;
	virtual USkeletalMeshComponent* GetCharacterMesh() const override;
	virtual FTransform GetWeaponLeftHandSocketTransform() const override;
	virtual ETurningInPlace GetTurningInPlace() const override;
	// Anim Interface

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	UPROPERTY(ReplicatedUsing = OnRep_AbilitySystemComponent)
		TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
		TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "Abilities")
		TArray < TSubclassOf < class UGameplayAbility >> StartupAbilities;

	// 패션 skeletalmesh 관련 필요. 

	UFUNCTION()
		virtual void OnRep_AbilitySystemComponent() {}

	virtual void BeginPlay() override;

	void AddCharacterAbilities();

	void AimOffset(float DeltaTime);
	void TurnInPlace(float DeltaTime);

	void SetCharacterStance(ECharacterStance ToStance);
private:

	// Anim variable
	float Lean;
	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;

	UPROPERTY(Replicated)
	float Yaw;
	// 서버에서 클라이언트로 Yaw 값을 동기화하기 위한 RPC 함수 선언
	UFUNCTION(Client, Reliable)
		void ClientSetYaw(float NewYaw);
	void SetYaw(float NewYaw);
	float InterpYaw;

	float Pitch;
	FRotator StartingAimRotation;

	UPROPERTY(Replicated)
	ECharacterStance CharacterStance = ECharacterStance::Stand;
	UFUNCTION(Client, Reliable)
		void ClientSetCharacterStance(ECharacterStance ToStance);

	UPROPERTY(BlueprintReadWrite, Category = Test, meta = (AllowPrivateAccess = "true")) // 나중에 bp에 노출 안할 거
		bool bIsAiming = false;

	UPROPERTY(Replicated)
	ETurningInPlace TurningInPlace = ETurningInPlace::TIP_NotTurning;
	UFUNCTION(Client, Reliable)
		void ClientSetTIP(ETurningInPlace NewTIP);
	void SetTIP(ETurningInPlace NewTIP);
	// Anim variable



};
