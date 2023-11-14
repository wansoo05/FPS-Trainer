// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WidgetManager.generated.h"

UCLASS()
class PROJECT_API AWidgetManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWidgetManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void CreateGameScore();
	void CreateAnalysisReport();
	void CreateSoundAlarm();
	void CreateAimTrainingHUD();

	void AddtoViewGameScore();
	void AddtoViewAnalysisReport();
	void AddtoViewSoundAlarm();

	void RemoveSoundAlarm();


	class UGameScore* GetGameScoreWidget();
	class UAnalysisWidget* GetAnalysisReportWidget();
	class UAimTrainingHUD* GetAimTrainingHUDWidget();

	TSubclassOf<UUserWidget> GameScoreClass;
	class UGameScore* GameScoreWidget;

	TSubclassOf<UUserWidget> AnalysisReportClass;
	class UAnalysisWidget* AnalysisReportWidget;

	TSubclassOf<UUserWidget> SoundAlarmClass;
	class UUserWidget* SoundAlarmWidget;

	TSubclassOf<UUserWidget> AimTrainingHUDClass;
	class UAimTrainingHUD* AimTrainingHUDWidget;

	class AprojectCharacter* PlayerCharacter;

	class AAnalysisManager* AnalysisManager;


	bool isCreateSoundWidget = false;
};
