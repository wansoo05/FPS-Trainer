// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AISoundPlayAssist.generated.h"

UCLASS()
class PROJECT_API AAISoundPlayAssist : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAISoundPlayAssist();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
