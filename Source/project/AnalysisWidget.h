// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AnalysisWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UAnalysisWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PistolAccuracyTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RifleAccuracyTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SniperAccuracyTextBlock;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PistolDistanceTextBlock;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* RifleDistanceTextBlock;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SniperDistanceTextBlock;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HPCountTextBlock;

	void SetReport(float PistolAccuracy, float RifleAccuracy, float SniperAccuracy,
				float PistolAverageDistance, float RifleAverageDistance, float SniperAverageDistance,
				int HPCount);

	class AWidgetManager* WidgetManager;
};
