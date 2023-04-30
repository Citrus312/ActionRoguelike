// Fill out your copyright notice in the Description page of Project Settings.


#include "YPotion_Health.h"

AYPotion_Health::AYPotion_Health()
{
	HealthRecovery = 50.0f;
}

void AYPotion_Health::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AYPotion_Health::OnComponentBeginOverlap);
}

void AYPotion_Health::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UYAttributeComponent* AttributeComp = Cast<UYAttributeComponent>(OtherActor->GetComponentByClass(UYAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			HideAndCooldownPowerup();
			ConsumeAudioComp->Activate(true);
			AttributeComp->ApplyHealthChange(this, HealthRecovery);
		}
	}
}
