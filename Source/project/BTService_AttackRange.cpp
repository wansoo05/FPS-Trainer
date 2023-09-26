// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_AttackRange.h"
#include "projectAIController.h"
#include "projectCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_AttackRange::UBTService_AttackRange()
{
	NodeName = TEXT("IsAttack");
	Interval = 0.5f;
}
void UBTService_AttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return;

	auto Target = Cast<AprojectCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AprojectAIController::TargetKey));
	if (nullptr == Target)
		return;

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AprojectAIController::IsAttackKey, Target->GetDistanceTo(ControllingPawn) <= 600.0f);
	UE_LOG(LogTemp, Warning, TEXT("%d"), OwnerComp.GetBlackboardComponent()->GetValueAsBool(AprojectAIController::IsAttackKey));
}
