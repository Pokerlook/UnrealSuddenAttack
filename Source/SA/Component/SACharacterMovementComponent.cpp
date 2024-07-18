// Fill out your copyright notice in the Description page of Project Settings.


#include "SACharacterMovementComponent.h"
#include "GameFramework/Character.h"

USACharacterMovementComponent::USACharacterMovementComponent()
{
	NavAgentProps.bCanCrouch = true;
}

USACharacterMovementComponent::FSavedMove_SA::FSavedMove_SA()
{

}

bool USACharacterMovementComponent::FSavedMove_SA::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	// 현재 동작과 새 동작 확인, 결합할 수 있는지. 가능하면 두 움직임이 하나의 패킷으로 결합되어 서버로 전송, 대역폭 절약.
	FSavedMove_SA* NewSAMove = static_cast<FSavedMove_SA*>(NewMove.Get());
	
	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void USACharacterMovementComponent::FSavedMove_SA::Clear()
{
	Super::Clear();

}

uint8 USACharacterMovementComponent::FSavedMove_SA::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();


	return Result;
}

void USACharacterMovementComponent::FSavedMove_SA::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	// 현재 캐릭터의 상태(위치, 속도, 방향, 입력 상태 등)를 기반으로 움직임 데이터를 저장
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	USACharacterMovementComponent* CharacterMovement = Cast<USACharacterMovementComponent>(C->GetCharacterMovement());
}

void USACharacterMovementComponent::FSavedMove_SA::PrepMoveFor(ACharacter* C)
{
	// 저장된 움직임 데이터를 캐릭터의 현재 상태로 복원, 클라이언트와 서버 간의 상태 동기화
	Super::PrepMoveFor(C);

	USACharacterMovementComponent* CharacterMovement = Cast<USACharacterMovementComponent>(C->GetCharacterMovement());

}

USACharacterMovementComponent::FNetworkPredictionData_Client_SA::FNetworkPredictionData_Client_SA(const UCharacterMovementComponent& ClientMovement)
	:Super(ClientMovement)
{
}

FSavedMovePtr USACharacterMovementComponent::FNetworkPredictionData_Client_SA::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_SA());
}

FNetworkPredictionData_Client* USACharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);

	if (ClientPredictionData == nullptr)
	{
		USACharacterMovementComponent* MutableThis = const_cast<USACharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_SA(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void USACharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
}

void USACharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	
}
