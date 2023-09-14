// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScore.h"
#include "Components/TextBlock.h"

void UGameScore::NativeConstruct()
{
	FString DefualtScore = FString::Printf(TEXT("0"));

	MyScore->SetText(FText::FromString(DefualtScore));
	AIScore->SetText(FText::FromString(DefualtScore));
}
