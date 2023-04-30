// Fill out your copyright notice in the Description page of Project Settings.


#include "YProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AYProjectile::AYProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");//Projectile为预设好的碰撞
	//SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AYProjectile::OnComponentBeginOverlap);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	MoveAudioComp = CreateDefaultSubobject<UAudioComponent>("MoveAudioComp");
	MoveAudioComp->bAutoActivate = true;
	MoveAudioComp->SetupAttachment(RootComponent);

	ExplodeAudioComp = CreateDefaultSubobject<UAudioComponent>("ExplodeComp");
	ExplodeAudioComp->bAutoActivate = false;
	ExplodeAudioComp->SetupAttachment(RootComponent);
	ExplodeAudioComp->OnAudioFinished.AddDynamic(this, &AYProjectile::OnExplodeAudioFinished);
}

// Called when the game starts or when spawned
void AYProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

void AYProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, /*触发时间的主体，通常是控制的人 */ UPrimitiveComponent* OterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("The Projectile %s overlap the %s, at game time %f"),*GetNameSafe(this), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
}

void AYProjectile::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void AYProjectile::OnExplodeAudioFinished()
{
	this->Destroy();
}

void AYProjectile::Explode()
{
	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);
	
	MoveAudioComp->Stop();
	ExplodeAudioComp->Activate(true);
}

void AYProjectile::Die()
{
	this->Destroy();
}

// Called every frame
void AYProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

