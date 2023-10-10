// Fill out your copyright notice in the Description page of Project Settings.


#include "AnalysisWidget.h"
#include "WidgetManager.h"

#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UAnalysisWidget::SetReport(float PistolAccuracy, float RifleAccuracy, float SniperAccuracy,
	float PistolAverageDistance, float RifleAverageDistance, float SniperAverageDistance,
	int HPCount)
{
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWidgetManager::StaticClass(), FoundActors);
	WidgetManager = Cast<AWidgetManager>(FoundActors[0]);
	
	PistolAccuracyTextBlock->SetText(FText::AsNumber(PistolAccuracy));
	RifleAccuracyTextBlock->SetText(FText::AsNumber(RifleAccuracy));
	SniperAccuracyTextBlock->SetText(FText::AsNumber(SniperAccuracy));
}
