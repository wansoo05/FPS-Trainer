// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "projectAIController.h"
#include "projectCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto projectCharacter = Cast<AprojectCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == projectCharacter)
		return EBTNodeResult::Failed;

	projectCharacter->Attack();
	IsAttacking = true;
	projectCharacter->OnAttackEnd.AddLambda([this]() -> void {IsAttacking = false; });

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
