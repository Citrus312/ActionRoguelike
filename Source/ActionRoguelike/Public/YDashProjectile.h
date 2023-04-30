// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "YProjectile.h"
#include "YDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AYDashProjectile : public AYProjectile
{
	GENERATED_BODY()

protected:
	FTimerHandle TimerHandle_Explode;
	
public:
	// Sets default values for this actor's properties
	AYDashProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	UParticleSystemComponent* TeleportEffectComp;
	
	virtual void BeginPlay() override;

	void OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	void Explode() override;

	void TeleportInstigator();
};
