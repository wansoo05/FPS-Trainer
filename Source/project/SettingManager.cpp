// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingManager.h"
#include "GameFramework/Pawn.h"
#include "projectCharacter.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ASettingManager::ASettingManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASettingManager::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectCharacter = Cast<AprojectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));


}

// Called every frame
void ASettingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASettingManager::ControlMouseSensitivity(int i)
{
	ProjectCharacter->AddControllerPitchInput(i * 5.f);
	ProjectCharacter->AddControllerYawInput(i * 5.f);
}

