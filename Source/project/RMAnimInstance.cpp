// Fill out your copyright notice in the Description page of Project Settings.


#include "RMAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

URMAnimInstance::URMAnimInstance()
{
	currentPawnSpeed = 0.0f;
	isInAir = false;
	myMoveType = EMoveType::Pistol;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/mixamo/Remy/animation/Rifle/Fire_Rifle_Ironsights_Montage.Fire_Rifle_Ironsights_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Fire!"));
		attackMontage = ATTACK_MONTAGE.Object;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Fail!"));
	}
}

void URMAnimInstance::NativeUpdateAnimation(float deltaSeconds)
{
	Super::NativeUpdateAnimation(deltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		currentPawnSpeed = Pawn->GetVelocity().Size();
		currentDirection = CalculateDirection(Pawn->GetVelocity(),Pawn->GetActorRotation());
		auto character = Cast<ACharacter>(Pawn);
		if (character)
		{
			isInAir = character->GetMovementComponent()->IsFalling();
		}
	}
}

void URMAnimInstance::playAttackMontage()
{
	Montage_Play(attackMontage, 1.0f);
}

void URMAnimInstance::setMoveType(int Num)
{
	switch (Num)
	{
	case 1:
		myMoveType = EMoveType::Pistol;
		break;
	case 2:
		myMoveType = EMoveType::rifleSniper;
		break;
	case 3:
		myMoveType = EMoveType::rifleSniper;
		break;
	}
}
