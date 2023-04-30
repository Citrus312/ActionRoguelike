// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "YAttributeComponent.h"
#include "YAICharacter.generated.h"

class UPawnSensingComponent;
class UUserWidget;
class UYWorldUserWidget;

UCLASS()
class ACTIONROGUELIKE_API AYAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AYAICharacter();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UYWorldUserWidget* ActiveHealthBar;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	FName TimeToHitParaName;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UYAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UYAttributeComponent* OwningComp, float NewHealth, float Delta);

	void SetTargetActor(AActor* NewTarget);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

};
