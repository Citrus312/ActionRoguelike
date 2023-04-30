// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "YProjectile.h"
#include "YBlackHoleProjectile.generated.h"

class URadialForceComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AYBlackHoleProjectile : public AYProjectile
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AYBlackHoleProjectile();

protected:

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ForceComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,    //触发时间的主体，通常是控制的人
		UPrimitiveComponent* OterComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
