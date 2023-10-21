// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "projectAIController.h"
#include "projectCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto projectCharacter = Cast<AprojectCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	bool isStop = projectCharacter->GetIsStop();

	if (isStop) return EBTNodeResult::Failed;

	if (nullptr == projectCharacter)
		return EBTNodeResult::Failed;

	auto Target = Cast<AprojectCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AprojectAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	if (Target->GetDistanceTo(projectCharacter) <= 1000.0f)
	{
		projectCharacter->SetWeaponState(1);
		projectCharacter->WeaponChange(0);
	}
	else if (Target->GetDistanceTo(projectCharacter) <= 3000.0f)
	{
		projectCharacter->SetWeaponState(2);
		projectCharacter->WeaponChange(0);
	}
	else
	{
		projectCharacter->SetWeaponState(3);
		projectCharacter->WeaponChange(0);
	}
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
