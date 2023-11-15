// Copyright Epic Games, Inc. All Rights Reserved.

#include "projectCharacter.h"
#include "ReadWriteJson.h"
#include "GameScore.h"
#include "projectAIController.h"
#include "RMAnimInstance.h"
#include "AnalysisManager.h"
#include "AnalysisWidget.h"
#include "WidgetManager.h"
#include "SettingManager.h"
#include "projectAIController.h"
#include "NavigationSystem.h"
#include "Bullet.h"
#include "projectGameMode.h"

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
#include "DrawDebugHelpers.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense.h"
#include "Perception/AIPerceptionComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"

#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include <cstdlib>
#include <ctime>


//////////////////////////////////////////////////////////////////////////
// AprojectCharacter

AprojectCharacter::AprojectCharacter()
{
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

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MouseSensitivity(
		TEXT("/Game/ThirdPerson/Input/Actions/IA_MouseSensitivity.IA_MouseSensitivity"));
	if (IA_MouseSensitivity.Succeeded())
	{
		MouseSensitivityAction = IA_MouseSensitivity.Object;
	}

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	FName WeaponSocket(TEXT("pistol"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		//UE_LOG(LogTemp, Warning, TEXT("WeaponSocket exist"));
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


	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	EmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComponent"));

	TSubclassOf<UAISense_Hearing> Hearing;
	
	AIPerceptionComponent->SetDominantSense(Hearing);

	AIControllerClass = AprojectAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TeamId = FGenericTeamId(0);
}

void AprojectCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	RMAnim = Cast<URMAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == RMAnim) return;

	RMAnim->OnMontageEnded.AddDynamic(this, &AprojectCharacter::onAttackMontageEnded);
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AprojectCharacter::onPerceptionUpdated);
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
	if (IsAttacking || isStop) return;

	RMAnim->PlayAttackMontage();
	IsAttacking = true;

	FRotator MuzzleRotation = Camera->GetComponentRotation();
	FVector MuzzleLocation = Camera->GetComponentLocation() + MuzzleRotation.RotateVector(FVector(50.0f, 0.0f, 0.0f));

	UWorld* World = GetWorld();

	this->ShootCount += 1;

	if (ProjectileClass)
	{
		World->SpawnActor<ABullet>(ProjectileClass, MuzzleLocation, MuzzleRotation);
	}

	GunAudioComp->Play(0.f);
	MakeNoise(1.0f);
	EmitterComponent->MakeNoise(this, 1.0f, GetActorForwardVector());

	if (!(this->IsPlayerControlled())) {
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, GetController(), 0.0f);
	}
}

void AprojectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWidgetManager::StaticClass(), FoundActors);

	if(FoundActors.Num() > 0) 
		WidgetManager = Cast<AWidgetManager>(FoundActors[0]);
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAnalysisManager::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
		AnalysisManager = Cast<AAnalysisManager>(FoundActors[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettingManager::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
		SettingManager = Cast<ASettingManager>(FoundActors[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
		PlayerTargetPoint = Cast<ATargetPoint>(FoundActors[0]);
	for (int i = 1; i < FoundActors.Num(); i++)
	{
		AITargetPoint[i] = Cast<ATargetPoint>(FoundActors[i]);
	}
	

	if (WidgetManager != nullptr)
	{
		if (this->IsPlayerControlled()) {
			WidgetManager->CreateGameScore();
			WidgetManager->CreateAnalysisReport();
			WidgetManager->AddtoViewGameScore();
			UE_LOG(LogTemp, Warning, TEXT("Create Success"));
		}

		GameScoreWidget = WidgetManager->GetGameScoreWidget();
		AnalysisReportWidget = WidgetManager->GetAnalysisReportWidget();
	}

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

void AprojectCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UE_LOG(LogTemp, Warning, TEXT("EndPlay!"));

	
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
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AprojectCharacter::FireEnd);

		//Zooming
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AprojectCharacter::Zoom);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &AprojectCharacter::ZoomOut);

		//Run
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AprojectCharacter::RunStart);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AprojectCharacter::RunStop);

		//Changing
		EnhancedInputComponent->BindAction(WeaponChangeUPAction, ETriggerEvent::Triggered, this, &AprojectCharacter::WeaponChangeUP);
		EnhancedInputComponent->BindAction(WeaponChangeDownAction, ETriggerEvent::Triggered, this, &AprojectCharacter::WeaponChangeDown);

		EnhancedInputComponent->BindAction(MouseSensitivityAction, ETriggerEvent::Triggered, this, &AprojectCharacter::ControlMouseSensitivity);
		
	}
}

void AprojectCharacter::CalculateHP(int Value)
{
	HP = HP += Value;
	if (HP < 1) Die();
}

void AprojectCharacter::HitActor(AActor* OtherActor)
{
	//AprojectCharacter* HitCharacter = Cast<AprojectCharacter>(OtherActor);

	if (isStop) return;
	FVector DistanceVector = this->GetActorLocation() - OtherActor->GetActorLocation();
	float Distance = DistanceVector.Size();
	AprojectCharacter* HitCharacter = Cast<AprojectCharacter>(OtherActor);


	if (this->IsPlayerControlled() && AnalysisManager != nullptr) {
		if (OtherActor->GetClass() == this->GetClass())
		{
			HitCount += 1;
			HitCharacter->CalculateHP(-1);
			AnalysisManager->An_AddData(WeaponState, true, Distance);
		}
		else
		{
			AnalysisManager->An_AddData(WeaponState, false, Distance);
		}
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
	LookAxisVector = LookAxisVector * Sensitivity;

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AprojectCharacter::Fire(const FInputActionValue& Value)
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

void AprojectCharacter::FireEnd(const FInputActionValue& Value)
{
	//IsAttacking = false;
}

void AprojectCharacter::Zoom(const FInputActionValue& Value)
{
	if (WeaponState == 3) {
		Camera->SetFieldOfView(20.0f);
	}
	else if (WeaponState == 2) {
		Camera->SetFieldOfView(50.0f);
	}
	else if (WeaponState == 1) {
		Camera->SetFieldOfView(90.0f);
	}
}

void AprojectCharacter::ZoomOut(const FInputActionValue& Value)
{
	Camera->SetFieldOfView(90.0f);
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
		ProjectileClass.GetDefaultObject()->SetBulletSpeed(8000.f);
		FName WeaponSocket(TEXT("pistol"));
		if (GetMesh()->DoesSocketExist(WeaponSocket))
		{
			UStaticMesh* SM_Gun = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Game/Weapon/Gun/gun/SMg18")));
			Weapon->SetStaticMesh(SM_Gun);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		}
	}
	else if (WeaponState == 2) {
		ProjectileClass.GetDefaultObject()->SetBulletSpeed(13000.f);
		FName WeaponSocket(TEXT("rifle"));
		if (GetMesh()->DoesSocketExist(WeaponSocket))
		{
			UStaticMesh* SM_Rifle = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Game/Weapon/Riple/scarL")));
			if (Weapon == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Weapon is nullptr!!"));
				return;
			}
			Weapon->SetStaticMesh(SM_Rifle);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
			//UE_LOG(LogTemp, Warning, TEXT("%s"), Weapon);
		}
	}
	else if (WeaponState == 3) {
		ProjectileClass.GetDefaultObject()->SetBulletSpeed(18000.f);
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

int AprojectCharacter::GetWeaponState()
{
	return WeaponState;
}

void AprojectCharacter::SetWeaponState(int Num)
{
	WeaponState = Num;
}

void AprojectCharacter::onAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsAttacking) return;
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

bool AprojectCharacter::GetIsStop()
{
	return isStop;
}

void AprojectCharacter::ControlMouseSensitivity(const FInputActionValue& Value)
{
	FVector2D ControlVector = Value.Get<FVector2D>();
	
	if (ControlVector.X == 1.f) {
		Sensitivity -= 0.1f;
	}
	else {
		Sensitivity += 0.1f;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Current Sensitivity: %f"), Sensitivity);
}

void AprojectCharacter::Die()
{
	/* Add isGround Check */

	if (isStop) return;
	isStop = true;

	RMAnim->PlayDieMontage();

	if (this->IsPlayerControlled()) {
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		DisableInput(PlayerController);
		WidgetManager->GameScoreWidget->ScoreUP(1);
	}
	else {
		WidgetManager->GameScoreWidget->ScoreUP(0);
	}
	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AprojectCharacter::Respawn, 3, false);
}

void AprojectCharacter::Respawn()
{
	HP = MaxHP;
	int RandomNumber{};

	if (this->IsPlayerControlled()) {
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		EnableInput(PlayerController);
		SetActorLocation(PlayerTargetPoint->GetActorLocation());
	}
	else {
		srand((unsigned int)time(NULL));
		RandomNumber = (int)rand() % 10;
		SetActorLocation(AITargetPoint[RandomNumber]->GetActorLocation());
	}

	isStop = false;


	//if (this->IsPlayerControlled())
	//	AI->isStop = false;
	//else
	//	Player->isStop = false;
}

bool AprojectCharacter::isHitAim()
{
	FRotator MuzzleRotation = Camera->GetComponentRotation();
	FVector MuzzleLocation = Camera->GetComponentLocation();

	return false;
}

void AprojectCharacter::onPerceptionUpdated(const TArray<AActor*>& DetectedPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("I Can Hear"));
	UE_LOG(LogTemp, Warning, TEXT("I Can Hear"));
}

void AprojectCharacter::DetectSound()
{
	if (isTrainingMode) {
		UE_LOG(LogTemp, Warning, TEXT("DetectSound"));
		if (this->IsPlayerControlled()) {
			if (WidgetManager != nullptr) {
				WidgetManager->AddtoViewSoundAlarm();

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AprojectCharacter::RemoveSoundAlarm, 3, false);
			}
		}
	}
}

void AprojectCharacter::RemoveSoundAlarm()
{
	WidgetManager->RemoveSoundAlarm();
}

AAnalysisManager* AprojectCharacter::GetAnalysisManager()
{
	return AnalysisManager;
}





