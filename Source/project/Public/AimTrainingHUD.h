// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AimTrainingHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UAimTrainingHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SensitivityTextBlock;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FstRecmdTextBlock;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SndRecmdTextBlock;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TrdRecmdTextBlock;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FstHitCountTextBlock;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SndHitCountTextBlock;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TrdHitCountTextBlock;

	void SetTextBlock(float Sensitivity, TMap<float, int32> Recommend);
};
