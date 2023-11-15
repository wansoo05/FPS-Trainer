// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AnalysisManager.generated.h"

UCLASS()
class PROJECT_API AAnalysisManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAnalysisManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void An_FireCount();
	void An_Weapon(int Weapon);
	void An_HitCount(bool Hit);
	void An_HPCount();

	void An_AddData(int Weapon, bool Hit, float Distance);
	void An_AddData(int Weapon, bool Hit);

	void An_CalculateData();

	void An_PrintData();

	float FireCount = 0;
	float HitCount = 0;

	float PistolFireCount = 0;
	float RifleFireCount = 0;
	float SniperFireCount = 0;

	float PistolHitCount = 0;
	float RifleHitCount = 0;
	float SniperHitCount = 0;

	float PistolDistance = 0;
	float RifleDistance = 0;
	float SniperDistance = 0;

	float HPCount = 0;

	float PistolAccuracy = 0;
	float RifleAccuracy = 0;
	float SniperAccuracy = 0;

	float PistolAverageDistance = 0;
	float RifleAverageDistance = 0;
	float SniperAverageDistance = 0;
};
