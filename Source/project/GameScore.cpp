// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScore.h"
#include "Components/TextBlock.h"

void UGameScore::NativeConstruct()
{
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
	}
	else if (Who == 1) {
		AIScore += 1;
		AIScoreText->SetText(FText::AsNumber(AIScore));
	}
}


