// Fill out your copyright notice in the Description page of Project Settings.


#include "YMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework//ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AYMagicProjectile::AYMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");//ProjectileΪԤ��õ���ײ
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}

// Called when the game starts or when spawned
void AYMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AYMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
