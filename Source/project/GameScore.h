// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameScore.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_API UGameScore : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();
	void Construct();

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MyScoreText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AIScoreText;

	float MyScore = 0;
	float AIScore = 0;

	// Who = 0 : MyScore += 1
	// Who = 1 : AIScore
	void ScoreUP(int Who);
};
