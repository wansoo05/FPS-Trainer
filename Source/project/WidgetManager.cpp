// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetManager.h"
#include "GameScore.h"
#include "AnalysisWidget.h"
#include "AnalysisManager.h"	
#include "projectCharacter.h"
#include "TrainingCharacter.h"
#include "AimTrainingHUD.h"

#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWidgetManager::AWidgetManager()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UGameScore> GameScoreAsset(TEXT("/Game/UMG/UMGGameScore.UMGGameScore_C"));
	static ConstructorHelpers::FClassFinder<UAnalysisWidget> AnalysisReportAsset(TEXT("/Game/UMG/UMGAnalysisReport.UMGAnalysisReport_C"));
	static ConstructorHelpers::FClassFinder<UUserWidget> SoundAlarmAsset(TEXT("/Game/UMG/UMGSoundAlaram.UMGSoundAlaram_C"));
	static ConstructorHelpers::FClassFinder<UAimTrainingHUD> AimTrainingHUDAsset(TEXT("/Game/UMG/UMGAimTrainingHUD.UMGAimTrainingHUD_C"));

	if (GameScoreAsset.Succeeded())
		GameScoreClass = GameScoreAsset.Class;

	if (AnalysisReportAsset.Succeeded())
		AnalysisReportClass = AnalysisReportAsset.Class;

	if (SoundAlarmAsset.Succeeded())
		SoundAlarmClass = SoundAlarmAsset.Class;

	if (AimTrainingHUDAsset.Succeeded())
	{
		AimTrainingHUDClass = AimTrainingHUDAsset.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The Class Lost"));
	}
}

// Called when the game starts or when spawned
void AWidgetManager::BeginPlay()
{
	Super::BeginPlay();

	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if (LevelName == "Stylized_Egypt_Demo")
	{
		PlayerCharacter = Cast<AprojectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		
		TArray<AActor*> FoundActors{};
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAnalysisManager::StaticClass(), FoundActors);
		AnalysisManager = Cast<AAnalysisManager>(FoundActors[0]);
		UE_LOG(LogTemp, Warning, TEXT("AnalysisManager: %s"), *(AnalysisManager->GetName()));
	}
	else if (LevelName == "AimTrainingMap")
	{
		//PlayerCharacter = Cast<ATrainingCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
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
		//AnalysisReportWidget->AddToViewport();
		//AnalysisReportWidget->RemoveFromParent();
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

void AWidgetManager::CreateAimTrainingHUD()
{
	if (IsValid(AimTrainingHUDClass))
	{
		AimTrainingHUDWidget = Cast<UAimTrainingHUD>(CreateWidget(GetWorld(), AimTrainingHUDClass));
	}
	else
	{
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
		AnalysisManager->An_CalculateData();

		AnalysisReportWidget->AddToViewport();

		AnalysisReportWidget->SetReport(AnalysisManager->PistolAccuracy, AnalysisManager->RifleAccuracy, AnalysisManager->SniperAccuracy,
			AnalysisManager->PistolAverageDistance, AnalysisManager->RifleAverageDistance,
			AnalysisManager->SniperAverageDistance, AnalysisManager->HPCount);
	}
}

void AWidgetManager::AddtoViewSoundAlarm()
{
	if (!isCreateSoundWidget) {
		CreateSoundAlarm();

		if (IsValid(SoundAlarmWidget))
		{
			SoundAlarmWidget->AddToViewport();
			isCreateSoundWidget = true;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("There is no SoundAlarmWidget"));
		}
	}
}

void AWidgetManager::RemoveSoundAlarm()
{
	isCreateSoundWidget = false; 
	SoundAlarmWidget->RemoveFromViewport();
	UE_LOG(LogTemp, Warning, TEXT("RemoveSoundAlarm"));
}

UGameScore* AWidgetManager::GetGameScoreWidget()
{
	return GameScoreWidget;
}

UAnalysisWidget* AWidgetManager::GetAnalysisReportWidget()
{
	return AnalysisReportWidget;
}

UAimTrainingHUD* AWidgetManager::GetAimTrainingHUDWidget()
{
	return AimTrainingHUDWidget;
}

