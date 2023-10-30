// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Stop.h"
#include "projectAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "projectCharacter.h"

UBTService_Stop::UBTService_Stop()
{
	NodeName = TEXT("Stop");
	Interval = 0.5f;
}

void UBTService_Stop::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AprojectCharacter* ControllingCharacter = Cast<AprojectCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AprojectAIController::IsStopKey, ControllingCharacter->GetIsStop());
}
