// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "projectAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AprojectAIController : public AAIController
{
	GENERATED_BODY()
	
protected: 
	virtual void BeginPlay() override;

	FGenericTeamId TeamId;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:
	AprojectAIController();
	virtual void OnPossess(APawn* InPawn) override;

	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName IsStopKey;

	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=AI)
	float AISightRadius = 4000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightAge = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AILoseSightRadius = AISightRadius + 1000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AIFieldOfView = 90.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	class UAIPerceptionComponent* AIPerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	class UAISenseConfig_Sight* SightConfig;

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
