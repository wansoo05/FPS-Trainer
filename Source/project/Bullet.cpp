// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "projectCharacter.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	FireParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("FireParticleSystem"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Fire(TEXT("/Game/StarterContent/Particles/P_Explosion"));
	if (Fire.Succeeded())
	{
		FireParticle = Fire.Object;
	}

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->InitSphereRadius(15.0f);
	RootComponent = CollisionComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;
	ProjectileMovementComponent->InitialSpeed = 8000.0f;
	ProjectileMovementComponent->MaxSpeed = 8000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	//ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.5f;
	InitialLifeSpan = 3.0f;

	StaticMeshComponent->SetupAttachment(CollisionComponent);


}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (FireParticle == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("doesn't exist"));
	UGameplayStatics::SpawnEmitterAtLocation(this, FireParticle, Hit.ImpactPoint);
	AprojectCharacter* playerCharacter = Cast<AprojectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	playerCharacter->HitActor(OtherActor);
	if (OtherActor == playerCharacter)
	{
		if (OtherActor->GetClass() == playerCharacter->GetClass())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "player");
		}
	}
	else
	{
		if (OtherActor->GetClass() == playerCharacter->GetClass())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "other");
		}
	}
	Destroy();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Hit");
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
