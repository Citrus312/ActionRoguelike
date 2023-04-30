// Fill out your copyright notice in the Description page of Project Settings.


#include "YItemChest.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AYItemChest::AYItemChest()
{

	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComp"));
	RootComponent = BaseMeshComp;

	LidMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMeshComp"));
	LidMeshComp->SetupAttachment(BaseMeshComp);

	TargetPitch = 110.0f;
}

void AYItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMeshComp->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
	//throw std::logic_error("The method or operation is not implemented.");
}

