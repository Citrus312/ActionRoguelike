// Fill out your copyright notice in the Description page of Project Settings.


#include "YPotionBase.h"

// Sets default values
AYPotionBase::AYPotionBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	ConsumeAudioComp = CreateDefaultSubobject<UAudioComponent>("MoveAudioComp");
	ConsumeAudioComp->bAutoActivate = false;
	ConsumeAudioComp->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;
}

void AYPotionBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AYPotionBase::ShowPowerup()
{
	SetPowerupState(true);
}

void AYPotionBase::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AYPotionBase::ShowPowerup, RespawnTime);
}

void AYPotionBase::SetPowerupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);
	// Set visibility on root and all children
	RootComponent->SetVisibility(bNewIsActive, true);
}

