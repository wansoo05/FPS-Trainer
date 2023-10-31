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
	static ConstructorHelpers::FClassFinder<UUserWidget> SoundAlarmAsset(TEXT("/Game/UMG/UMGSoundAlaram.UMGSoundAlaram_C"));

	if (GameScoreAsset.Succeeded())
		GameScoreClass = GameScoreAsset.Class;

	if (AnalysisReportAsset.Succeeded())
		AnalysisReportClass = AnalysisReportAsset.Class;

	if (SoundAlarmAsset.Succeeded())
		SoundAlarmClass = SoundAlarmAsset.Class;
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

void AWidgetManager::CreateSoundAlarm()
{
	if (IsValid(SoundAlarmClass))
	{
		SoundAlarmWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), SoundAlarmClass));
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

		AnalysisReportWidget->AddToViewport();

		AnalysisReportWidget->SetReport(AnalysisManager->PistolAccuracy, AnalysisManager->RifleAccuracy, AnalysisManager->SniperAccuracy,
			AnalysisManager->PistolAverageDistance, AnalysisManager->RifleAverageDistance,
			AnalysisManager->SniperAverageDistance, AnalysisManager->HPCount);
	}
}

void AWidgetManager::AddtoViewSoundAlarm()
{
	if (IsValid(SoundAlarmWidget))
	{
		SoundAlarmWidget->AddToViewport();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("There is no SoundAlarmWidget"));
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

