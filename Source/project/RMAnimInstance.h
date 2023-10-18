// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RMAnimInstance.generated.h"
//#include "enumMoveType.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EMoveType : uint8
{
	Pistol UMETA(DisplayName = "Pistol"),
	rifleSniper UMETA(DisplayName = "rifleSniper")
};

UCLASS()
class PROJECT_API URMAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	URMAnimInstance();
	virtual void NativeUpdateAnimation(float deltaSeconds) override;

	void playAttackMontage();


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float currentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool isInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float currentDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	EMoveType myMoveType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* PistolAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RipleAttackMontage;

	UFUNCTION()
	void AnimNotify_AttackCheck();

public:
	void setMoveType(int Num);
};
