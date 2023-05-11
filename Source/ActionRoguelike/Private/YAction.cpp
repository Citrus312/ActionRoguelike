// Fill out your copyright notice in the Description page of Project Settings.


#include "YAction.h"
#include "YActionComponent.h"
#include "../ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

void UYAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

UYActionComponent* UYAction::GetOwningComponent() const
{
	return ActionComp;
}

void UYAction::Initialize(UYActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool UYAction::IsRunning() const
{
	return RepData.bIsRunning;
}

bool UYAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	UYActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockTags))
	{
		return false;
	}
	return true;
}

void UYAction::StartAction_Implementation(AActor* Instigator)
{

	UYActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

void UYAction::StopAction_Implementation(AActor* Instigator)
{

	UYActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
}

UWorld* UYAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

void UYAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UYAction, RepData);
	DOREPLIFETIME(UYAction, ActionComp);
}