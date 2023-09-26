// Copyright Epic Games, Inc. All Rights Reserved.

#include "projectCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RMAnimInstance.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "projectAIController.h"



//////////////////////////////////////////////////////////////////////////
// AprojectCharacter

AprojectCharacter::AprojectCharacter()
{

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate


	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

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

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Jump(
		TEXT("/Game/ThirdPerson/Input/Actions/IA_Jump.IA_Jump"));
	if (IA_Jump.Succeeded())
	{
		JumpAction = IA_Jump.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Look(
		TEXT("/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look"));
	if (IA_Look.Succeeded())
	{
		LookAction = IA_Look.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Move(
		TEXT("/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move"));
	if (IA_Move.Succeeded())
	{
		MoveAction = IA_Move.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Fire(
		TEXT("/Game/ThirdPerson/Input/Actions/IA_Fire.IA_Fire"));
	if (IA_Fire.Succeeded())
	{
		FireAction = IA_Fire.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Zoom(
		TEXT("/Game/ThirdPerson/Input/Actions/IA_Zoom.IA_Zoom"));
	if (IA_Zoom.Succeeded())
	{
		ZoomAction = IA_Zoom.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Run(
		TEXT("/Game/ThirdPerson/Input/Actions/IA_Run.IA_Run"));
	if (IA_Run.Succeeded())
	{
		RunAction = IA_Run.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ChangeWeaponUP(
		TEXT("/Game/ThirdPerson/Input/Actions/IA_ChangeWeaponUP.IA_ChangeWeaponUP"));
	if (IA_ChangeWeaponUP.Succeeded())
	{
		WeaponChangeUPAction = IA_ChangeWeaponUP.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_ChangeWeaponDown(
		TEXT("/Game/ThirdPerson/Input/Actions/IA_ChangeWeaponDown.IA_ChangeWeaponDown"));
	if (IA_ChangeWeaponDown.Succeeded())
	{
		WeaponChangeDownAction = IA_ChangeWeaponDown.Object;
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

	AIControllerClass = AprojectAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AprojectCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	RMAnim = Cast<URMAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == RMAnim) return;

	RMAnim->OnMontageEnded.AddDynamic(this, &AprojectCharacter::onAttackMontageEnded);
}

void AprojectCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!IsPlayerControlled())
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	}
}

void AprojectCharacter::Attack()
{
	if (IsAttacking) return;

	RMAnim->playAttackMontage();
	IsAttacking = true;

	FHitResult OutHit;
	FVector Start = Camera->GetComponentLocation();
	FVector ForwardVector = Camera->GetForwardVector();
	FVector End = ((ForwardVector * 5000.f) + Start);
	FCollisionQueryParams CollisionParams;

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	this->ShootCount += 1;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (OutHit.bBlockingHit)
		{
			// 
			if (this->IsPlayerControlled()) {
				if (OutHit.GetActor()->GetClass() == this->GetClass()) {
					AprojectCharacter* AI = Cast<AprojectCharacter>(OutHit.GetActor());
					AI->CalculateHP(-1);
					this->HitCount += 1;
					UE_LOG(LogTemp, Warning, TEXT("AI HP: %d"), AI->HP);
				}
			}
			else {
				if (OutHit.GetActor()->GetClass() == this->GetClass()) {
					AprojectCharacter* ControlledPawn = Cast<AprojectCharacter>(OutHit.GetActor());
					ControlledPawn->CalculateHP(-1);
					UE_LOG(LogTemp, Warning, TEXT("My HP: %d"), ControlledPawn->HP);
				}
			}
		}
	}
}

void AprojectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	Camera = this->FindComponentByClass<UCameraComponent>();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AprojectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AprojectCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AprojectCharacter::Look);

		//Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AprojectCharacter::Fire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AprojectCharacter::FireEnd);

		//Zooming
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AprojectCharacter::Zoom);

		//Run
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AprojectCharacter::RunStart);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AprojectCharacter::RunStop);

		//Changing
		EnhancedInputComponent->BindAction(WeaponChangeUPAction, ETriggerEvent::Triggered, this, &AprojectCharacter::WeaponChangeUP);
		EnhancedInputComponent->BindAction(WeaponChangeDownAction, ETriggerEvent::Triggered, this, &AprojectCharacter::WeaponChangeDown);
	}
}

void AprojectCharacter::CalculateHP(int Value)
{
	HP = HP += Value;
	if (HP < 1) Die();
}

void AprojectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AprojectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AprojectCharacter::Fire(const FInputActionValue& Value)
{
	Attack();
}

void AprojectCharacter::FireEnd(const FInputActionValue& Value)
{
	IsAttacking = false;
}

void AprojectCharacter::Zoom(const FInputActionValue& Value)
{
}

void AprojectCharacter::RunStart(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 800.0f;
}

void AprojectCharacter::RunStop(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void AprojectCharacter::Load(const FInputActionValue& Value)
{
	if (WeaponState == 1) {
		PistolBullet = MaxPistolBullet;
	}
	else if (WeaponState == 2) {
		RifleBullet = MaxRifleBullet;
	}
	else if (WeaponState == 3) {
		SniperBullet = MaxSniperBullet;
	}

	UE_LOG(LogTemp, Warning, TEXT("Load!"));
}

void AprojectCharacter::WeaponChangeUP(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("UP"));
	WeaponChange(1);
}

void AprojectCharacter::WeaponChangeDown(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("DOWN"));
	WeaponChange(-1);
}

void AprojectCharacter::WeaponChange(int Num)
{
	/* Num = 1 : UP
	   Num = -1 : Down 
	   WeaponState 1: Gun 2: Rifle 3: Sniper */

	WeaponState += Num;

	if (WeaponState == 0) WeaponState = 3;
	else if (WeaponState == 4) WeaponState = 1;

	RMAnim->setMoveType(WeaponState);

	if (WeaponState == 1) {
		FName WeaponSocket(TEXT("pistol"));
		if (GetMesh()->DoesSocketExist(WeaponSocket))
		{
			UStaticMesh* SM_Gun = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Game/Weapon/Gun/gun/SMg18")));
			Weapon->SetStaticMesh(SM_Gun);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		}
	}
	else if (WeaponState == 2) {
		FName WeaponSocket(TEXT("rifle"));
		if (GetMesh()->DoesSocketExist(WeaponSocket))
		{
			UStaticMesh* SM_Rifle = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Game/Weapon/Riple/scarL")));
			Weapon->SetStaticMesh(SM_Rifle);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
			UE_LOG(LogTemp, Warning, TEXT("%s"), Weapon);
		}
	}
	else if (WeaponState == 3) {
		FName WeaponSocket(TEXT("sniper"));
		if (GetMesh()->DoesSocketExist(WeaponSocket))
		{
			UStaticMesh* SM_Sniper = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Game/Weapon/Sniper/meshes/Sniper")));
			Weapon->SetStaticMesh(SM_Sniper);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("%d: Weapon Change ERROR!!!!!!!!!!!!"), WeaponState);
	}
}

void AprojectCharacter::onAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsAttacking) return;
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

void AprojectCharacter::Die()
{
	/* Add isGround Check */
	GetMesh()->PlayAnimation(DieAnim, false);
}





