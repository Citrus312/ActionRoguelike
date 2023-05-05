// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YPotionBase.h"
#include "YPotion_Health.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AYPotion_Health : public AYPotionBase
{
	GENERATED_BODY()
public:

	AYPotion_Health();
	
protected:
	float HealthRecovery;
	int32 CreditCost;

	void Interact_Implementation(APawn* InstigatorPawn) override;
};
