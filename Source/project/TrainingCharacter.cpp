// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainingCharacter.h"
#include "TrainingPlayerController.h"
#include "RMAnimInstance.h"
#include "TargetActor.h"
#include "AimTrainingHUD.h"
#include "WidgetManager.h"
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

	IsRight = false;

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
	ProjectileClass.GetDefaultObject()->SetBulletSpeed(20000);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWidgetManager::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
		WidgetManager = Cast<AWidgetManager>(FoundActors[0]);

	if (WidgetManager != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("widgetAlive"));
		WidgetManager->CreateAimTrainingHUD();
		AimTrainingHUDWidget = WidgetManager->GetAimTrainingHUDWidget();
		if (AimTrainingHUDWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("HUDAlive"));
			AimTrainingHUDWidget->AddToViewport();
			AimTrainingHUDWidget->SetTextBlock(Sensitivity, Recommend);
		}
	}
}

void ATrainingCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ATrainingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CameraRotation = Camera->GetComponentRotation();
	if (ArCameraRotators[CameraRoatorPos] != CameraRotation)
	{
		CameraRoatorPos++;
		if (CameraRoatorPos >= 1000)
		{
			CameraRoatorPos = 0;
		}

		ArCameraRotators[CameraRoatorPos] = CameraRotation;
	}
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

void ATrainingCharacter::SetMouseSensitivity(FVector HitDirection)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetActor::StaticClass(), FoundActors);
	ATargetActor* Target = Cast<ATargetActor>(FoundActors[0]);

	//float DistanceFromTargetToPlayer = Target->GetDistanceTo(Player);
	FVector TargetLocation = Target->GetActorLocation();
	FVector PlayerToTarget = TargetLocation - GetActorLocation();
	//float Sensitivity = 0;

	if (PlayerToTarget.Cross(HitDirection).Z > 0)
	{
		if (IsRight == false)
		{
			Sensitivity += 0.1;
		}
		else
		{
			Sensitivity -= 0.1;
		}
		UE_LOG(LogTemp, Warning, TEXT("Hit Right"));
	}
	else
	{
		if (IsRight == false)
		{
			Sensitivity -= 0.1;
		}
		else
		{
			Sensitivity += 0.1;
		}
		UE_LOG(LogTemp, Warning, TEXT("Hit Left"));
	}
	
	AimTrainingHUDWidget->SetTextBlock(Sensitivity, Recommend);
}

void ATrainingCharacter::SetIsAttacking(bool New)
{
	IsAttacking = New;
}

void ATrainingCharacter::SetRecommend()
{
	int32* HitCount = Recommend.Find(Sensitivity);
	if (HitCount == nullptr)
	{
		Recommend.Add(Sensitivity, 1);
	}
	else
	{
		(*HitCount)++;
	}
	Recommend.ValueSort([](int32 A, int32 B) {return (A > B); });
	AimTrainingHUDWidget->SetTextBlock(Sensitivity, Recommend);
}


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
	if (IsAttacking) return;

	if (CameraRoatorPos == 0)
	{
		if (ArCameraRotators[CameraRoatorPos].Yaw < ArCameraRotators[999].Yaw)
		{
			IsRight = false;
		}
		else
		{
			IsRight = true;
		}
	}
	else
	{
		if (ArCameraRotators[CameraRoatorPos].Yaw < ArCameraRotators[CameraRoatorPos - 1].Yaw)
		{
			IsRight = false;
		}
		else
		{
			IsRight = true;
		}
	}
	Attack();
}

void ATrainingCharacter::FireEnd(const FInputActionValue& Value)
{
}

void ATrainingCharacter::onAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsAttacking) return;
	//IsAttacking = false;
	OnAttackEnd.Broadcast();
}