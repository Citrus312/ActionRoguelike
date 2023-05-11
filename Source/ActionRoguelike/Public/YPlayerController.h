// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "YPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AYPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChange;

	virtual void SetPawn(APawn* InPawn) override;
};
