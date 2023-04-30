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

void AYPotionBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, /*触发时间的主体，通常是控制的人 */ UPrimitiveComponent* OterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
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

