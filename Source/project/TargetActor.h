// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetActor.generated.h"

UCLASS()
class PROJECT_API ATargetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	void RandomSpawn();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = CollisionComponent)
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnyWhere, Category = STATICMESH)
	UStaticMeshComponent* StaticMeshTarget;

	bool IsDie;

	void Die();

};
