// Fill out your copyright notice in the Description page of Project Settings.


#include "YDashProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework//ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

AYDashProjectile::AYDashProjectile()
{
	DetonateDelay = 0.2f;
	TeleportDelay = 0.2f;

	TeleportEffectComp = CreateDefaultSubobject<UParticleSystemComponent>("TeleportEffectComp");
	TeleportEffectComp->bAutoActivate = false;
	TeleportEffectComp->SetupAttachment(RootComponent);

	MovementComp->InitialSpeed = 5000.0f;
}

void AYDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &AYDashProjectile::Explode, DetonateDelay);
	SphereComp->OnComponentHit.AddDynamic(this, &AYDashProjectile::OnActorHit);
}

void AYDashProjectile::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("  The Projectile %s hit the %s and explode"), *GetNameSafe(this), *GetNameSafe(OtherActor));
}

void AYDashProjectile::Explode()
{
	//Çå³ýTimer
	GetWorldTimerManager().ClearTimer(TimerHandle_Explode);

	Super::Explode();

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &AYDashProjectile::TeleportInstigator, TeleportDelay);
}

void AYDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		TeleportEffectComp->ActivateSystem(true);
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
	FTimerHandle TimerHandle_Die;
	GetWorldTimerManager().SetTimer(TimerHandle_Die, this, &AYDashProjectile::Die, 1.0f);
}

