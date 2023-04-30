// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "YAttributeComponent.h"
#include "YProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UAudioComponent;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API AYProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AYProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* MoveAudioComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* ExplodeAudioComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION() //一定要加，否则无效
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,    //触发时间的主体，通常是控制的人
		UPrimitiveComponent* OterComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnExplodeAudioFinished();

	UFUNCTION(BlueprintCallable)
	virtual void Explode();

	UFUNCTION()
	virtual void Die();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
