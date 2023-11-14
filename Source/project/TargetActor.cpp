// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetActor.h"
#include "TrainingCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Components/SphereComponent.h"

// Sets default values
ATargetActor::ATargetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Target"));
	CollisionComponent->InitSphereRadius(50.0f);
	RootComponent = CollisionComponent;

	StaticMeshTarget = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATICMESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_TARGET(TEXT("/Game/Level/Target2/10480_Archery_target_v1_max2011_iteration-2"));
	if (SM_TARGET.Succeeded())
	{
		StaticMeshTarget->SetStaticMesh(SM_TARGET.Object);
	}
	StaticMeshTarget->SetupAttachment(RootComponent);

	SetActorRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	IsDie = false;
}

// Called when the game starts or when spawned
void ATargetActor::BeginPlay()
{
	Super::BeginPlay();

	RandomSpawn();
}

void ATargetActor::RandomSpawn()
{
	SetActorHiddenInGame(false);
	IsDie = false;
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	ATrainingCharacter* PlayerCharacter = Cast<ATrainingCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	FNavLocation RandomSpwanLocation{};
	NavSystem->GetRandomPoint(RandomSpwanLocation);
	FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(RandomSpwanLocation.Location, PlayerCharacter->GetActorLocation());

	SetActorLocation(RandomSpwanLocation.Location);
	SetActorRotation(TargetRot);
	
	PlayerCharacter->SetIsAttacking(false);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATargetActor::RandomSpawn, 3, false);
}

// Called every frame
void ATargetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATargetActor::Die()
{
	if (IsDie == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Hit"));
		SetActorHiddenInGame(true);
		IsDie = true;
	}
}

