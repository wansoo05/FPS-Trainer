// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadWriteJson.h"

#include "JsonObjectConverter.h"

FJsonStruct UReadWriteJson::ReadStructFromJsonFile(FString JsonFilePath)
{
	TSharedPtr<FJsonObject> JsonObject = ReadJson(JsonFilePath);

	FJsonStruct RetJsonStruct;

	if (!FJsonObjectConverter::JsonObjectToUStruct<FJsonStruct>(JsonObject.ToSharedRef(), &RetJsonStruct))
	{
		return FJsonStruct();
	}

	return RetJsonStruct;
}

void UReadWriteJson::WriteStructFromJsonFile(FString JsonFilePath, FJsonStruct Struct)
{
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Struct);

	if (JsonObject == nullptr) {
		return;
	}

	WriteJson(JsonFilePath, JsonObject);

	UE_LOG(LogTemp, Warning, TEXT("Write Json!"));
}

TSharedPtr<FJsonObject> UReadWriteJson::ReadJson(FString JsonFilePath)
{
	FString JsonString = "";

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*JsonFilePath))
	{
		return nullptr;
	}

	if (!FFileHelper::LoadFileToString(JsonString, *JsonFilePath))
	{
		return nullptr;
	}
	
	TSharedPtr<FJsonObject> RetJsonObject;

	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), RetJsonObject))
	{
		return nullptr;
	}

	return RetJsonObject;
}

void UReadWriteJson::WriteJson(FString JsonFilePath, TSharedPtr<FJsonObject> JSonObject)
{
	FString JsonString = "";

	if (!FJsonSerializer::Serialize(JSonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString, 0)))
	{
		return;
	}

	if (!FFileHelper::SaveStringToFile(JsonString, *JsonFilePath))
	{
		return;
	}

}