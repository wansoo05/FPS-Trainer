// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetManager.h"
#include "GameScore.h"
#include "AnalysisWidget.h"
#include "AnalysisManager.h"	
#include "projectCharacter.h"

#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWidgetManager::AWidgetManager()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UGameScore> GameScoreAsset(TEXT("/Game/UMG/UMGGameScore.UMGGameScore_C"));
	static ConstructorHelpers::FClassFinder<UAnalysisWidget> AnalysisReportAsset(TEXT("/Game/UMG/UMGAnalysisReport.UMGAnalysisReport_C"));

	if (GameScoreAsset.Succeeded())
		GameScoreClass = GameScoreAsset.Class;

	if (AnalysisReportAsset.Succeeded())
		AnalysisReportClass = AnalysisReportAsset.Class;
}

// Called when the game starts or when spawned
void AWidgetManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AprojectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AnalysisManager = PlayerCharacter->GetAnalysisManager();
}

// Called every frame
void AWidgetManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWidgetManager::CreateGameScore()
{
	if (IsValid(GameScoreClass))
	{
		GameScoreWidget = Cast<UGameScore>(CreateWidget(GetWorld(), GameScoreClass));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("The Widget Lost"));
	}
}

void AWidgetManager::CreateAnalysisReport()
{
	if (IsValid(AnalysisReportClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Create AnalysisWidget 1"));
		AnalysisReportWidget = Cast<UAnalysisWidget>(CreateWidget(GetWorld(), AnalysisReportClass));
		AnalysisReportWidget->AddToViewport();
		AnalysisReportWidget->RemoveFromParent();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("The Widget Lost"));
	}
}

void AWidgetManager::AddtoViewGameScore()
{
	if (IsValid(GameScoreWidget))
	{
		GameScoreWidget->AddToViewport();
	}
}

void AWidgetManager::AddtoViewAnalysisReport()
{
	if (AnalysisReportWidget)
	{
		AnalysisManager = PlayerCharacter->GetAnalysisManager();
		AnalysisManager->An_CalculateData();

		AnalysisReportWidget->SetReport(AnalysisManager->PistolAccuracy, AnalysisManager->RifleAccuracy, AnalysisManager->SniperAccuracy,
			AnalysisManager->PistolAverageDistance, AnalysisManager->RifleAverageDistance, 
			AnalysisManager->SniperAverageDistance, AnalysisManager->HPCount);

		AnalysisReportWidget->AddToViewport();
	}
}

UGameScore* AWidgetManager::GetGameScoreWidget()
{
	return GameScoreWidget;
}

UAnalysisWidget* AWidgetManager::GetAnalysisReportWidget()
{
	return AnalysisReportWidget;
}

