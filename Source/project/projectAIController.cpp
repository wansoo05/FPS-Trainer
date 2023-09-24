// Fill out your copyright notice in the Description page of Project Settings.


#include "projectAIController.h"

#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


const FName AprojectAIController::HomePosKey(TEXT("HomePos"));
const FName AprojectAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AprojectAIController::TargetKey(TEXT("Target"));
//void AprojectAIController::BeginPlay()
//{
//	Super::BeginPlay();
//
//}

AprojectAIController::AprojectAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/BB_projectCharacter.BB_projectCharacter"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/BT_projectCharacter.BT_projectCharacter"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void AprojectAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	UBlackboardComponent* TmpBB = Cast<UBlackboardComponent>(Blackboard);
	if (UseBlackboard(BBAsset, TmpBB))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("AIController coudln't run behavior tree!"));
		}
	}
}