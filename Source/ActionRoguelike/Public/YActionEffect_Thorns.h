// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YActionEffect.h"
#include "YActionEffect_Thorns.generated.h"

class UYAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API UYActionEffect_Thorns : public UYActionEffect
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float ThornsConvertRatio;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UYAttributeComponent* OwningComp, float NewHealth, float Delta);
	
public:

	UYActionEffect_Thorns();

	void StartAction_Implementation(AActor* Instigator) override;
	void StopAction_Implementation(AActor* Instigator) override;
};
