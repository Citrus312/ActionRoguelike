// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "YAttributeComponent.h"
#include "YPotionBase.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AYPotionBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AYPotionBase();

protected:

	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* ConsumeAudioComp;

	virtual void PostInitializeComponents() override;

	UFUNCTION() //一定要加，否则无效
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,    //触发时间的主体，通常是控制的人
		UPrimitiveComponent* OterComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void ShowPowerup();

	virtual void HideAndCooldownPowerup();

	virtual void SetPowerupState(bool bNewIsActive);
};
