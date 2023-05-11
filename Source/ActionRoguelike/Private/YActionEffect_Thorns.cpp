// Fill out your copyright notice in the Description page of Project Settings.


#include "YActionEffect_Thorns.h"
#include "YAttributeComponent.h"
#include "YGameplayFunctionLibrary.h"
#include "YActionComponent.h"

void UYActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UYAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		float Damage = round(-Delta * ThornsConvertRatio);
		UYGameplayFunctionLibrary::ApplyDamage(GetOwningComponent()->GetOwner(), InstigatorActor, Damage);
		UYGameplayFunctionLibrary::ApplyRage(GetOwningComponent()->GetOwner(), InstigatorActor, 1.0f);
	}
}

UYActionEffect_Thorns::UYActionEffect_Thorns()
{
	ThornsConvertRatio = 1.0f;
}

void UYActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	if (UYActionComponent* AcctionComp = GetOwningComponent())
	{
		if (AActor* MyActor = AcctionComp->GetOwner())
		{
			if (UYAttributeComponent* AttributeComp = UYAttributeComponent::GetAttributes(MyActor))
			{
				AttributeComp->OnHealthChanged.RemoveDynamic(this, &UYActionEffect_Thorns::OnHealthChanged);
			}
		}
	}
}

void UYActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (UYActionComponent* AcctionComp = GetOwningComponent())
	{
		if (AActor* MyActor = AcctionComp->GetOwner())
		{
			if (UYAttributeComponent* AttributeComp = UYAttributeComponent::GetAttributes(MyActor))
			{
				AttributeComp->OnHealthChanged.AddDynamic(this, &UYActionEffect_Thorns::OnHealthChanged);
// 				FString DebugMsg = "Action Effect " + GetNameSafe(this) + " bind!";
// 				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, DebugMsg);
			}
		}
	}
}


