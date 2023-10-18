// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReadWriteJson.generated.h"

class FJsonObject;

USTRUCT(BlueprintType)
struct FJsonStruct
{
	GENERATED_BODY()

public:
	/* FireCount : 발사 횟수
	*  HitCount : 명중 횟수
	*  Distance : 발사 시 AI와의 거리
	*  HPCount : 회복 아이템 사용 횟수
	*  Weapon : 사용한 총기 종류
	*/

	int FireCount = 0;
	int HitCount = 0;
	float Distance = 0.f;
	int HPCount = 0;
	int Weapon = 0;
};

UCLASS()
class PROJECT_API UReadWriteJson : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FJsonStruct ReadStructFromJsonFile(FString JsonFilePath);
	static void WriteStructFromJsonFile(FString JsonFilePath, FJsonStruct Struct);

public:
	static TSharedPtr<FJsonObject> ReadJson(FString JsonFilePath);
	static void WriteJson(FString JsonFilePath, TSharedPtr<FJsonObject> JSonObject);
};
