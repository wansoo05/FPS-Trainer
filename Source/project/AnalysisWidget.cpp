// Fill out your copyright notice in the Description page of Project Settings.


#include "AnalysisWidget.h"
#include "WidgetManager.h"

#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UAnalysisWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("HI!!!!!!!!!!!!!"));
}

void UAnalysisWidget::SetReport(float PistolAccuracy, float RifleAccuracy, float SniperAccuracy,
	float PistolAverageDistance, float RifleAverageDistance, float SniperAverageDistance,
	int HPCount)
{
	if (this != nullptr) {
		PistolAccuracyTextBlock->SetText(FText::AsNumber(PistolAccuracy));
		RifleAccuracyTextBlock->SetText(FText::AsNumber(RifleAccuracy));
		SniperAccuracyTextBlock->SetText(FText::AsNumber(SniperAccuracy));
		PistolDistanceTextBlock->SetText(FText::AsNumber(PistolAverageDistance));
		RifleDistanceTextBlock->SetText(FText::AsNumber(RifleAverageDistance));
		SniperDistanceTextBlock->SetText(FText::AsNumber(SniperAverageDistance));
		HPCountTextBlock->SetText(FText::AsNumber(HPCount));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("this is nullptr"));
	}
}
