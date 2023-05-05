// Fill out your copyright notice in the Description page of Project Settings.


#include "YPotion_Health.h"
#include "YPlayerState.h"

AYPotion_Health::AYPotion_Health()
{
	HealthRecovery = 50.0f;

	CreditCost = 50;
}

void AYPotion_Health::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		UYAttributeComponent* AttributeComp = UYAttributeComponent::GetAttributes(InstigatorPawn);
		if (ensure(AttributeComp))
		{
			if (AYPlayerState* PS = InstigatorPawn->GetPlayerState<AYPlayerState>())
			{
				if (PS->RemoveCredits(CreditCost))
				{
					HideAndCooldownPowerup();
					ConsumeAudioComp->Activate(true);
					AttributeComp->ApplyHealthChange(this, HealthRecovery);
				}
			}
		}
	}
}
