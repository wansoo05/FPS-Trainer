// Fill out your copyright notice in the Description page of Project Settings.


#include "AnalysisSystem.h"

AnalysisSystem::AnalysisSystem()
{
}

void AnalysisSystem::An_FireCount()
{
	FireCount += 1;
}

void AnalysisSystem::An_Weapon(int Weapon)
{
}

void AnalysisSystem::An_HitCount(bool Hit)
{
	HitCount += 1;
}

void AnalysisSystem::An_HPCount()
{
	HPCount += 1;
}

void AnalysisSystem::An_AddData(int Weapon, bool Hit, float Distance)
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

	case 2:
		RifleFireCount += 1;
		RifleDistance += Distance;
		if (Hit == true) {
			RifleHitCount += 1;
		}

	case 3:
		SniperFireCount += 1;
		SniperDistance += Distance;
		if (Hit == true) {
			SniperHitCount += 1;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Gun: %d, %d"), PistolFireCount, PistolHitCount);
}

void AnalysisSystem::An_CalculateData()
{
	PistolAccuracy = PistolHitCount / PistolFireCount;
	RifleAccuracy = RifleHitCount / RifleFireCount;
	SniperAccuracy = SniperHitCount / SniperFireCount;

	PistolAverageDistance = PistolDistance / PistolFireCount;
	RifleAverageDistance = RifleDistance / RifleFireCount;
	SniperAverageDistance = SniperDistance / SniperFireCount;
}

void AnalysisSystem::An_PrintData()
{
}
