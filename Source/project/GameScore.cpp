// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScore.h"
#include "WidgetManager.h"

#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UGameScore::NativeConstruct()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWidgetManager::StaticClass(), FoundActors);
	WidgetManager = Cast<AWidgetManager>(FoundActors[0]);
}

void UGameScore::Construct()
{

}

void UGameScore::ScoreUP(int Who)
{
	FText TestText = FText::FromString("TestString");

	if (Who == 0) {
		MyScore += 1;
		MyScoreText->SetText(FText::AsNumber(MyScore));
		if (MyScore == 10) {
			WidgetManager->AddtoViewAnalysisReport();
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
	else if (Who == 1) {
		AIScore += 1;
		AIScoreText->SetText(FText::AsNumber(AIScore));
		if (AIScore == 10) {
			WidgetManager->AddtoViewAnalysisReport();
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
}


