// Fill out your copyright notice in the Description page of Project Settings.


#include "YItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AYItemChest::AYItemChest()
{

	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComp"));
	RootComponent = BaseMeshComp;

	LidMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMeshComp"));
	LidMeshComp->SetupAttachment(BaseMeshComp);

	TargetPitch = 110.0f;
	bLidOpened = false;

	//update the client's chest
	SetReplicates(true);
}

void AYItemChest::OnRep_LidOpen()
{
	float CurPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMeshComp->SetRelativeRotation(FRotator(CurPitch, 0, 0));
}

void AYItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
}

void AYItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Notify all clients
	DOREPLIFETIME(AYItemChest, bLidOpened);
}
