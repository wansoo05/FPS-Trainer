// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Stop.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UBTService_Stop : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_Stop();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
