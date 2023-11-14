// Fill out your copyright notice in the Description page of Project Settings.


#include "AimTrainingHUD.h"
#include "Components/TextBlock.h"

void UAimTrainingHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAimTrainingHUD::SetTextBlock(float Sensitivity, TMap<float, int32> Recommend)
{
	if (this != nullptr)
	{
		SensitivityTextBlock->SetText(FText::AsNumber(Sensitivity));

		if (Recommend.Num() > 0)
		{
			auto Iter = Recommend.CreateConstIterator();
			FstRecmdTextBlock->SetText(FText::AsNumber(Iter->Key));
			FstHitCountTextBlock->SetText(FText::AsNumber(Iter->Value));
			++Iter;
			if (Recommend.Num() > 1)
			{
				SndRecmdTextBlock->SetText(FText::AsNumber(Iter->Key));
				SndHitCountTextBlock->SetText(FText::AsNumber(Iter->Value));
				++Iter;
			}
			if (Recommend.Num() > 2)
			{
				TrdRecmdTextBlock->SetText(FText::AsNumber(Iter->Key));
				TrdHitCountTextBlock->SetText(FText::AsNumber(Iter->Value));
			}
		}
	}
}
