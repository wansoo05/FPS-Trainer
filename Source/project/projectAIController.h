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
};
