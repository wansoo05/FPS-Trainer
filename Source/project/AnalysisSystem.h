// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROJECT_API AnalysisSystem
{
public:
	AnalysisSystem();

	void An_FireCount();
	void An_Weapon(int Weapon);
	void An_HitCount(bool Hit);
	void An_HPCount();

	void An_AddData(int Weapon, bool Hit, float Distance);
	
	void An_CalculateData();

	void An_PrintData();

	int FireCount = 0;
	int HitCount = 0;

	int PistolFireCount = 0;
	int RifleFireCount = 0;
	int SniperFireCount = 0;

	int PistolHitCount = 0;
	int RifleHitCount = 0;
	int SniperHitCount = 0;

	float PistolDistance = 0;
	float RifleDistance = 0;
	float SniperDistance = 0;

	int HPCount = 0;

	float PistolAccuracy = 0;
	float RifleAccuracy = 0;
	float SniperAccuracy = 0;

	float PistolAverageDistance = 0;
	float RifleAverageDistance = 0;
	float SniperAverageDistance = 0;
};
