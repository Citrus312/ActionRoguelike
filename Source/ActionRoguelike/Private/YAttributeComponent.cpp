// Fill out your copyright notice in the Description page of Project Settings.


#include "YAttributeComponent.h"
#include "YGameModeBase.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UYAttributeComponent::UYAttributeComponent()
{
	HealthMax = 120.0f;
	Health = HealthMax;
}

UYAttributeComponent* UYAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UYAttributeComponent>(FromActor->GetComponentByClass(UYAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool UYAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UYAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float UYAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool UYAttributeComponent::IsActorAlive(AActor* FromActor)
{
	UYAttributeComponent* AttributeComp = GetAttributes(FromActor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

bool UYAttributeComponent::ApplyHealthChange(AActor* Instigator, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	OnHealthChanged.Broadcast(Instigator, this, Health, Delta);

	if (Health == 0.0f && Delta < 0.0f)
	{
		AYGameModeBase* GM = GetWorld()->GetAuthGameMode<AYGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), Instigator);
		}
	}

	return true;
}

bool UYAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator, -GetHealthMax());
}