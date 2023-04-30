// Fill out your copyright notice in the Description page of Project Settings.


#include "YMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework//ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "YGameplayFunctionLibrary.h"

// Sets default values
AYMagicProjectile::AYMagicProjectile()
{
	ExplodeEffectComp = CreateDefaultSubobject<UParticleSystemComponent>("ExplodeEffectComp");
	ExplodeEffectComp->bAutoActivate = false;
	ExplodeEffectComp->SetupAttachment(RootComponent);

	MovementComp->InitialSpeed = 2000.0f;
	Damage = 20.0f;
}

void AYMagicProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UE_LOG(LogTemp, Warning, TEXT("The Projectile %s damage the %s, at game time %f"), *GetNameSafe(this), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
		if (UYGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult))
		{
			Explode();
		}
	}
}

void AYMagicProjectile::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("  The Projectile %s hit the %s and explode"), *GetNameSafe(this), *GetNameSafe(OtherActor));
	Explode();
	
}

// Called when the game starts or when spawned
void AYMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AYMagicProjectile::OnComponentBeginOverlap);
	SphereComp->OnComponentHit.AddDynamic(this, &AYMagicProjectile::OnActorHit);

	this->SetLifeSpan(20.0f);
}

void AYMagicProjectile::Explode()
{
	Super::Explode();
	
	ExplodeEffectComp->ActivateSystem(true);

	FTimerHandle TimerHandle_Die;
	GetWorldTimerManager().SetTimer(TimerHandle_Die, this, &AYMagicProjectile::Die, 1.0f);
}

