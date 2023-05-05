// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "YAttributeComponent.h"
#include "YGameplayInterface.h"
#include "YPotionBase.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AYPotionBase : public AActor, public IYGameplayInterface
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

	UFUNCTION()
	virtual void ShowPowerup();

	virtual void HideAndCooldownPowerup();

	virtual void SetPowerupState(bool bNewIsActive);
};
