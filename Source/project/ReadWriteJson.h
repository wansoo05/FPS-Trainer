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
	/* FireCount : �߻� Ƚ��
	*  HitCount : ���� Ƚ��
	*  Distance : �߻� �� AI���� �Ÿ�
	*  HPCount : ȸ�� ������ ��� Ƚ��
	*  Weapon : ����� �ѱ� ����
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
