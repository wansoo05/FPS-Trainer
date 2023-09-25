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
	//virtual void BeginPlay() override;

public:
	AprojectAIController();
	virtual void OnPossess(APawn* InPawn) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
