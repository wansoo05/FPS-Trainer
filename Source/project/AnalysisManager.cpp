// Fill out your copyright notice in the Description page of Project Settings.


#include "AnalysisManager.h"

// Sets default values
AAnalysisManager::AAnalysisManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAnalysisManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAnalysisManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAnalysisManager::An_FireCount()
{
	FireCount += 1;
}

void AAnalysisManager::An_Weapon(int Weapon)
{
}

void AAnalysisManager::An_HitCount(bool Hit)
{
	HitCount += 1;
}

void AAnalysisManager::An_HPCount()
{
	HPCount += 1;
}

void AAnalysisManager::An_AddData(int Weapon, bool Hit, float Distance)
{
	An_FireCount();

	switch (Weapon)
	{
	case 1:
		PistolFireCount += 1;
		PistolDistance += Distance;
		if (Hit == true) {
			PistolHitCount += 1;
		}
		break;

	case 2:
		RifleFireCount += 1;
		RifleDistance += Distance;
		if (Hit == true) {
			RifleHitCount += 1;
		}
		break;

	case 3:
		SniperFireCount += 1;
		SniperDistance += Distance;
		if (Hit == true) {
			SniperHitCount += 1;
		}
		break;
	}
}

void AAnalysisManager::An_CalculateData()
{

	PistolAccuracy = PistolHitCount / PistolFireCount;
	RifleAccuracy = RifleHitCount / RifleFireCount;
	SniperAccuracy = SniperHitCount / SniperFireCount;

	PistolAverageDistance = PistolDistance / PistolFireCount;
	RifleAverageDistance = RifleDistance / RifleFireCount;
	SniperAverageDistance = SniperDistance / SniperFireCount;

	UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f, %f"), PistolAccuracy, RifleAccuracy, PistolAverageDistance, RifleAverageDistance);
}

void AAnalysisManager::An_PrintData()
{
}
