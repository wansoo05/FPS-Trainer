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
	void AddtoViewGameScore();
	void AddtoViewAnalysisReport();

	class UGameScore* GetGameScoreWidget();
	class UAnalysisWidget* GetAnalysisReportWidget();

	TSubclassOf<UUserWidget> GameScoreClass;
	class UGameScore* GameScoreWidget;

	TSubclassOf<UUserWidget> AnalysisReportClass;
	class UAnalysisWidget* AnalysisReportWidget;

	class AprojectCharacter* PlayerCharacter;

	class AnalysisSystem* AnalysisSystem;
};
