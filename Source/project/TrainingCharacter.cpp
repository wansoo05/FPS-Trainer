// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainingCharacter.h"
#include "TrainingPlayerController.h"
#include "RMAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATrainingCharacter::ATrainingCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_REMY(TEXT("/Game/mixamo/Remy/mesh/Remy"));
	if (SK_REMY.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_REMY.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> REMY_ANIM(TEXT("/Game/mixamo/Remy/animation/RemyAnim.RemyAnim_C"));
	if (REMY_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(REMY_ANIM.Class);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Look(
		TEXT("/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look"));
	if (IA_Look.Succeeded())
	{
		LookAction = IA_Look.Object;
	}

	//static ConstructorHelpers::FObjectFinder<UInputAction>IA_Move(
	//	TEXT("/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move"));
	//if (IA_Move.Succeeded())
	//{
	//	MoveAction = IA_Move.Object;
	//}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Fire(
		TEXT("/Game/ThirdPerson/Input/Actions/IA_Fire.IA_Fire"));
	if (IA_Fire.Succeeded())
	{
		FireAction = IA_Fire.Object;
	}

	FName WeaponSocket(TEXT("pistol"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		//UE_LOG(LogTemp, Warning, TEXT("weapon!"));
		Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WEAPON(TEXT("/Game/Weapon/Gun/gun/SMg18"));
		if (SM_WEAPON.Succeeded())
		{
			Weapon->SetStaticMesh(SM_WEAPON.Object);
		}
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}
	IsAttacking = false;

	GunAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("GunAudioComponent"));
	GunAudioComp->SetupAttachment(Weapon);
}

void ATrainingCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	RMAnim = Cast<URMAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == RMAnim) return;

	RMAnim->OnMontageEnded.AddDynamic(this, &ATrainingCharacter::onAttackMontageEnded);
}

void ATrainingCharacter::Attack()
{
	if (IsAttacking) return;

	RMAnim->PlayAttackMontage();
	IsAttacking = true;

	FRotator MuzzleRotation = Camera->GetComponentRotation();
	FVector MuzzleLocation = Camera->GetComponentLocation() + MuzzleRotation.RotateVector(FVector(50.0f, 0.0f, 0.0f));

	UWorld* World = GetWorld();


	if (ProjectileClass)
	{
		World->SpawnActor<ABullet>(ProjectileClass, MuzzleLocation, MuzzleRotation);
	}

	GunAudioComp->Play(0.f);
}
// Called when the game starts or when spawned
void ATrainingCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	Camera = this->FindComponentByClass<UCameraComponent>();
}

// Called every frame
void ATrainingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATrainingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Moving
		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATrainingCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATrainingCharacter::Look);

		//Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATrainingCharacter::Fire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ATrainingCharacter::FireEnd);

	}
}

void ATrainingCharacter::HitActor(AActor* OtherActor)
{

}

//void ATrainingCharacter::Move(const FInputActionValue& Value)
//{
//	// input is a Vector2D
//	FVector2D MovementVector = Value.Get<FVector2D>();
//
//	if (Controller != nullptr)
//	{
//		// find out which way is forward
//		const FRotator Rotation = Controller->GetControlRotation();
//		const FRotator YawRotation(0, Rotation.Yaw, 0);
//
//		// get forward vector
//		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
//
//		// get right vector 
//		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
//
//		// add movement 
//		AddMovementInput(ForwardDirection, MovementVector.Y);
//		AddMovementInput(RightDirection, MovementVector.X);
//	}
//}

void ATrainingCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D

	FVector2D LookAxisVector = Value.Get<FVector2D>();
	LookAxisVector = LookAxisVector * Sensitivity;

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
void ATrainingCharacter::Fire(const FInputActionValue& Value)
{
	//if (this->IsPlayerControlled()) 
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Player HP: %d"), HP);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("AI HP: %d"), HP);
	//}

	Attack();
}

void ATrainingCharacter::FireEnd(const FInputActionValue& Value)
{
	//IsAttacking = false;
}

void ATrainingCharacter::onAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsAttacking) return;
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}