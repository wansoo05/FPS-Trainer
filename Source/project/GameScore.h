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

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MyScore;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AIScore;
};
