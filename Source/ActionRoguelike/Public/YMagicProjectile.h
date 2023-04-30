// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "YProjectile.h"
#include "YMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AYMagicProjectile : public AYProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AYMagicProjectile();

protected:

	UPROPERTY(EditDefaultsOnly)
	UParticleSystemComponent* ExplodeEffectComp;

	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Explode() override;

	UPROPERTY(EditDefaultsOnly)
	float Damage;

};
