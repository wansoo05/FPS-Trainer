// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnalysisJson.generated.h"

/**
 * 
 */

struct FJsonStruct;

UCLASS()
class PROJECT_API UAnalysisJson : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*
	UFUNCTION(BlueprintCallable)
		static TMap<FString, FJsonStruct> ReadStructFromJsonFile_DataTableFormat(FString FilePath, bool& isOutSuccess, FString& OutInfoMessage);

	UFUNCTION(BlueprintCallable)
		static void WriteStructFromJsonFile_DataTableFormat(FString FilePath, TMap<FString, FJsonStruct> RowsToStruct, bool& isOutSuccess, FString& OutInfoMessage);
		*/
};
