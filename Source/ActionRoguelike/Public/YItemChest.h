// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YGameplayInterface.h"
#include "YItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API AYItemChest : public AActor, public IYGameplayInterface
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere)
	float TargetPitch;

	// Sets default values for this actor's properties
	AYItemChest();

protected:

	// RepNotify
	UPROPERTY(ReplicatedUsing = "OnRep_LidOpen", BlueprintReadOnly)
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpen();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMeshComp;

public:	


	void Interact_Implementation(APawn* InstigatorPawn) override;

};
