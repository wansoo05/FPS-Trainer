// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<UStaticMeshComponent> ST_Bullet(TEXT("/Game/Weapon/Gun/bullet/9mm_bullet.9mm_bullet"));
	//if (ST_Bullet.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(SK_REMY.Object);
	//}

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	FireParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("FireParticle"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Fire(TEXT("/Game/InfinityBladeEffects/Effects/FX_Ambient/Fire/P_LavaFire_PourBase"));
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
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	//ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.5f;
	InitialLifeSpan = 3.0f;

	StaticMeshComponent->SetupAttachment(CollisionComponent);

	CollisionComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);



}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GameStatic->SpawnEmitterAtLocation(this, FireParticle, Hit.ImpactPoint);
	Destroy();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Overlapped");
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
