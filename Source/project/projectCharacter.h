// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "projectCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS(config=Game)
class AprojectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/** Zoom Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ZoomAction;

	/** Run Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RifleAction;

public:
	AprojectCharacter();
	
public:
	virtual void PostInitializeComponents() override;
	UPROPERTY(VisibleAnyWhere, Category = Weapon)
	UStaticMeshComponent* Weapon;
	virtual void PossessedBy(AController* NewController) override;

	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

protected:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void FireEnd(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);

	void RunStart(const FInputActionValue& Value);
	void RunStop(const FInputActionValue& Value);

	void Load(const FInputActionValue& Value);

	void WeaponChangeUP(const FInputActionValue& Value);
	void WeaponChangeDown(const FInputActionValue& Value);
	void WeaponChange(int Num);

	void Die();

	class UInputAction* WeaponChangeUPAction;
	class UInputAction* WeaponChangeDownAction;
	class UInputAction* LoadAction;
	class UAnimSequence* DieAnim;

	UFUNCTION()
	void onAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputMappingContext* DefaultContext;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking = false;

	UPROPERTY()
	class URMAnimInstance* RMAnim;

	/* HP = HP + Value */
	void CalculateHP(int Value);

	UPROPERTY(VisibleAnywhere)
	int HP = 100;

	int WeaponState = 1;

	float ShootRate = 0.f;
	int ShootCount = 0;
	int HitCount = 0;

	int PistolBullet = 10;
	int RifleBullet = 30;
	int SniperBullet = 2;

	int MaxPistolBullet = 10;
	int MaxRifleBullet = 30;
	int MaxSniperBullet = 2;

	class UCameraComponent* Camera;
};

