// Fill out your copyright notice in the Description page of Project Settings.


#include "YMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework//ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "YGameplayFunctionLibrary.h"
#include "YActionComponent.h"
#include "YActionEffect.h"

// Sets default values
AYMagicProjectile::AYMagicProjectile()
{
	ExplodeEffectComp = CreateDefaultSubobject<UParticleSystemComponent>("ExplodeEffectComp");
	ExplodeEffectComp->bAutoActivate = false;
	ExplodeEffectComp->SetupAttachment(RootComponent);

	MovementComp->InitialSpeed = 2000.0f;
	Damage = 20.0f;

	ImpactShakeInnerRadius = 0.0f;
	ImpactShakeOuterRadius = 1500.0f;
}

void AYMagicProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UYActionComponent* ActionComp = Cast<UYActionComponent>(OtherActor->GetComponentByClass(UYActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		//UE_LOG(LogTemp, Warning, TEXT("The Projectile %s damage the %s, at game time %f"), *GetNameSafe(this), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
		if (UYGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult))
		{
			// Rage
			UYGameplayFunctionLibrary::ApplyRage(GetInstigator(), OtherActor, 1.0f);

			Explode();

			if (ActionComp && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
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
	UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);


	FTimerHandle TimerHandle_Die;
	GetWorldTimerManager().SetTimer(TimerHandle_Die, this, &AYMagicProjectile::Die, 1.0f);
}

