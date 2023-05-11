// Fill out your copyright notice in the Description page of Project Settings.


#include "YGameplayFunctionLibrary.h"
#include "YAttributeComponent.h"

bool UYGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UYAttributeComponent* AttributeComp = UYAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, -1 * DamageAmount);
	}
	return false;
}

bool UYGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 3000.0f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}

bool UYGameplayFunctionLibrary::ApplyRage(AActor* RageCauser, AActor* TargetActor, float RageAmount)
{
	UYAttributeComponent* AttributeComp = UYAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyRageChange(RageCauser, RageAmount);
	}
	return false;
}
