// Fill out your copyright notice in the Description page of Project Settings.


#include "YAttributeComponent.h"
#include "YGameModeBase.h"
#include "Net/UnrealNetwork.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UYAttributeComponent::UYAttributeComponent()
{
	HealthMax = 120.0f;
	Health = HealthMax;
	RageMax = 10.0f;
	Rage = 0.0f;
	RageConvertRatio = 0.25f;

	SetIsReplicatedByDefault(true);
}

UYAttributeComponent* UYAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UYAttributeComponent>(FromActor->GetComponentByClass(UYAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool UYAttributeComponent::AddRage(float Delta)
{
	if (Delta > 0.0f)
	{
		float DeltaRage = round(Delta * RageConvertRatio);
		Rage = FMath::Clamp(Rage + DeltaRage, 0.0f, RageMax);
		return true;
	}
	return false;
}

bool UYAttributeComponent::IsRageMax() const
{
	return Rage == RageMax;
}

void UYAttributeComponent::ClearRage()
{
	Rage = 0.0f;
}

void UYAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void UYAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
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

	if (!IsAlive())
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (NewHealth != OldHealth)
		{
			MulticastHealthChanged(Instigator, Health, Delta);
		}

		// Died
		if (Health == 0.0f && Delta < 0.0f)
		{
			AYGameModeBase* GM = GetWorld()->GetAuthGameMode<AYGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), Instigator);
			}
		}
	}
	
	return OldHealth != NewHealth;
}

bool UYAttributeComponent::ApplyRageChange(AActor* Instigator, float Delta)
{
	if (!IsAlive())
	{
		return false;
	}

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	MulticastRageChanged(Instigator, Rage, Delta);

	return true;
}

bool UYAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator, -GetHealthMax());
}

void UYAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UYAttributeComponent, Health);
	DOREPLIFETIME(UYAttributeComponent, HealthMax);
	//DOREPLIFETIME_CONDITION(UYAttributeComponent, HealthMax, COND_OwnerOnly);
}