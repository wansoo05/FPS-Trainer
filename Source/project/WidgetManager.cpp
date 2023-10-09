// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetManager.h"
#include "GameScore.h"
#include "AnalysisWidget.h"
#include "AnalysisSystem.h"	
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
	AnalysisSystem* AnSys = PlayerCharacter->GetAnalysisSystem();
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
		AnalysisReportWidget = Cast<UAnalysisWidget>(CreateWidget(GetWorld(), AnalysisReportClass));
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
	if (IsValid(AnalysisReportWidget))
	{
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

