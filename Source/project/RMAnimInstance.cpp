// Fill out your copyright notice in the Description page of Project Settings.


#include "RMAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

URMAnimInstance::URMAnimInstance()
{
	currentPawnSpeed = 0.0f;
	isInAir = false;
}

void URMAnimInstance::NativeUpdateAnimation(float deltaSeconds)
{
	Super::NativeUpdateAnimation(deltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		currentPawnSpeed = Pawn->GetVelocity().Size();
		auto character = Cast<ACharacter>(Pawn);
		if (character)
		{
			isInAir = character->GetMovementComponent()->IsFalling();
		}
	}
}
