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
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_REMY(TEXT("/Game/mixamo/Remy/mesh/Remy"));
	if (SK_REMY.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_REMY.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> REMY_ANIM(TEXT("/Game/mixamo/Remy/animation/RemyAnimBlueprint.RemyAnimBlueprint_C"));
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

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Rifle(
		TEXT("/Game/ThirdPerson/Input/Actions/IA_Rifle.IA_Rifle"));
	if (IA_Rifle.Succeeded())
	{
		RifleAction = IA_Rifle.Object;
	}

	IsAttacking = false;

	FName WeaponSocket(TEXT("weapon"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WEAPON(TEXT("/Game/Weapon/Riple/scarL"));
		if (SM_WEAPON.Succeeded())
		{
			//UE_LOG(LogTemp, Warning, TEXT("weapon!"));
			Weapon->SetStaticMesh(SM_WEAPON.Object);
		}
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}
}

void AprojectCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	RMAnim = Cast<URMAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == RMAnim) return;

	RMAnim->OnMontageEnded.AddDynamic(this, &AprojectCharacter::onAttackMontageEnded);
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

		//Zooming
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AprojectCharacter::Zoom);

		//Run
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AprojectCharacter::RunStart);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AprojectCharacter::RunStop);

		EnhancedInputComponent->BindAction(RifleAction, ETriggerEvent::Triggered, this, &AprojectCharacter::AttachRifle);
	}
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
	if (IsAttacking) return;


	RMAnim->playAttackMontage();
	IsAttacking = true;
}

void AprojectCharacter::Zoom(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Zoom!"));
}

void AprojectCharacter::RunStart(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 800.0f;
}

void AprojectCharacter::RunStop(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void AprojectCharacter::AttachRifle(const FInputActionValue& Value)
{
}

void AprojectCharacter::onAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsAttacking) return;
	IsAttacking = false;
}





