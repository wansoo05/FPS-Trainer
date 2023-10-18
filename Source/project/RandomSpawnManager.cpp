// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomSpawnManager.h"

// Sets default values
ARandomSpawnManager::ARandomSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARandomSpawnManager::BeginPlay()
{
	Super::BeginPlay();


	FName path = TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter");

	UBlueprint* ObjectToSpawn =
		Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), NULL, *path.ToString()));

	FActorSpawnParameters spawnParams;

	float pitch = FMath::FRandRange(-180.f, 180.f);
	float yaw = FMath::FRandRange(-180.f, 180.f);
	float roll = FMath::FRandRange(-180.f, 180.f);

	FRotator rotator = FRotator(pitch, yaw, roll);

	float X = FMath::FRandRange(-4650.f, -6000.f);
	float Y = FMath::FRandRange(-2920.f, -5000.f);

	FVector spawnLocation = FVector(X, Y, 100.0f);

	GetWorld()->SpawnActor<AActor>(ObjectToSpawn->GeneratedClass, spawnLocation, rotator, spawnParams);
}

// Called every frame
void ARandomSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

