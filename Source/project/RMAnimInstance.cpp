// Fill out your copyright notice in the Description page of Project Settings.


#include "RMAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "projectCharacter.h"

URMAnimInstance::URMAnimInstance()
{
	currentPawnSpeed = 0.0f;
	isInAir = false;
	myMoveType = EMoveType::Pistol;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PISTOL_ATTACK_MONTAGE(TEXT("/Game/mixamo/Remy/animation/Pistol/Shooting_Montage"));
	if (PISTOL_ATTACK_MONTAGE.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire!"));
		PistolAttackMontage = PISTOL_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RIPLE_ATTACK_MONTAGE(TEXT("/Game/mixamo/Remy/animation/Rifle/Fire_Rifle_Ironsights_Montage"));
	if (PISTOL_ATTACK_MONTAGE.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire!"));
		RipleAttackMontage = RIPLE_ATTACK_MONTAGE.Object;
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
	int WeaponState{};
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		auto character = Cast<AprojectCharacter>(Pawn);
		if (character)
		{
			WeaponState = character->GetWeaponState();
		}
	}

	switch (WeaponState)
	{
	case 1:
		if (Montage_IsPlaying(PistolAttackMontage) == false)
			Montage_Play(PistolAttackMontage, 1.0f);
		break;
	case 2:
		if (Montage_IsPlaying(RipleAttackMontage) == false)
			Montage_Play(RipleAttackMontage, 1.0f);
		break;
	case 3:
		if (Montage_IsPlaying(RipleAttackMontage) == false)
			Montage_Play(RipleAttackMontage, 1.0f);
		break;
	}
}

void URMAnimInstance::AnimNotify_AttackCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack"));
}

void URMAnimInstance::setMoveType(int Num)
{
	switch (Num)
	{
	case 1:
		myMoveType = EMoveType::Pistol;
		//UE_LOG(LogTemp, Warning, TEXT("Pistol"));
		break;
	case 2:
		myMoveType = EMoveType::rifleSniper;
		//UE_LOG(LogTemp, Warning, TEXT("Rifle"));
		break;
	case 3:
		myMoveType = EMoveType::rifleSniper;
		//UE_LOG(LogTemp, Warning, TEXT("Sniper"));
		break;
	}
}
