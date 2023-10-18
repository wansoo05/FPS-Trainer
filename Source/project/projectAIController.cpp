// Fill out your copyright notice in the Description page of Project Settings.


#include "projectAIController.h"

#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "projectCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


const FName AprojectAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AprojectAIController::TargetKey(TEXT("Target"));
const FName AprojectAIController::IsAttackKey(TEXT("IsAttack"));
const FName AprojectAIController::IsSeeKey(TEXT("IsSee"));

void AprojectAIController::BeginPlay()
{
	Super::BeginPlay();

}

ETeamAttitude::Type AprojectAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);
	if (OtherPawn == nullptr)
		return ETeamAttitude::Neutral;

	auto PlayerTI = Cast<IGenericTeamAgentInterface>(&Other);
	class IGenericTeamAgentInterface* BotTI = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	if(BotTI == nullptr && PlayerTI == nullptr)
		return ETeamAttitude::Neutral;

	FGenericTeamId OtherActorTeamId = NULL;
	if (BotTI != nullptr)
	{
		OtherActorTeamId = BotTI->GetGenericTeamId();
	}
	else if (PlayerTI != nullptr)
	{
		OtherActorTeamId = PlayerTI->GetGenericTeamId();
	}

	FGenericTeamId ThisId = GetGenericTeamId();
	if (OtherActorTeamId == 255)
	{
		return ETeamAttitude::Neutral;
	}
	else if (OtherActorTeamId == ThisId)
	{
		return ETeamAttitude::Friendly;
	}
	else
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Type();
}

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


	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerComp->SetDominantSense(*SightConfig->GetSenseImplementation());
	AIPerComp->OnPerceptionUpdated.AddDynamic(this, &AprojectAIController::OnPawnDetected);
	AIPerComp->ConfigureSense(*SightConfig);

	TeamId = FGenericTeamId(1);
}

void AprojectAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	UBlackboardComponent* TmpBB = Cast<UBlackboardComponent>(Blackboard);
	if (UseBlackboard(BBAsset, TmpBB))
	{

		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("AIController coudln't run behavior tree!"));
		}
	}
}

void AprojectAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (int i = 0; i < DetectedPawns.Num(); i++)
	{
		if (IsValid(GetBlackboardComponent()->GetValueAsObject(AprojectAIController::TargetKey)))
		{
			UE_LOG(LogTemp, Warning, TEXT("Exit"));
			GetBlackboardComponent()->SetValueAsObject(AprojectAIController::TargetKey, NULL);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Enter, %s"), *DetectedPawns[i]->GetActorNameOrLabel());
			//AprojectCharacter* playerCharacter = Cast<AprojectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			GetBlackboardComponent()->SetValueAsObject(AprojectAIController::TargetKey, DetectedPawns[i]);
		}
	}
}