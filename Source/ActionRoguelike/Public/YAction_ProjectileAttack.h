// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YAction.h"
#include "Particles/ParticleSystem.h"
#include "YAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UYAction_ProjectileAttack : public UYAction
{
	GENERATED_BODY()

public:
	UYAction_ProjectileAttack();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FName SpawnLocationName;

	void GetAttackTM(ACharacter* Instigator, FTransform& SpawnTM);

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
	
	FTimerHandle TimerHandle_AttackDelay;

	float AttackAnimDelay;
	float AttackDistance;

public:

	virtual void StartAction_Implementation(AActor* Instigator) override;
};
