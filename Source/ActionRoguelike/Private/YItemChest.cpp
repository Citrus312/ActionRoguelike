// Fill out your copyright notice in the Description page of Project Settings.


#include "YItemChest.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AYItemChest::AYItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComp"));
	RootComponent = BaseMeshComp;

	LidMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMeshComp"));
	LidMeshComp->SetupAttachment(BaseMeshComp);

	TargetPitch = 110.0f;
}

// Called when the game starts or when spawned
void AYItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AYItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AYItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMeshComp->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
	//throw std::logic_error("The method or operation is not implemented.");
}

