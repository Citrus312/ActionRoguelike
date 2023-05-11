// Fill out your copyright notice in the Description page of Project Settings.


#include "YActionComponent.h"
#include "YAction.h"
#include "../ActionRoguelike.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

UYActionComponent::UYActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


void UYActionComponent::AddAction(AActor* Instigator, TSubclassOf<UYAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	UYAction* NewAction = NewObject<UYAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UYActionComponent::RemoveAction(UYAction* ActionToRemove)
{
	if (ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	Actions.Remove(ActionToRemove);
}

bool UYActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UYAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FailedMsg);
				continue;
			}

			// Is Client?
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}

			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UYActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UYAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName && Action->IsRunning())
		{
			// Is Client?
			if (!GetOwner()->HasAuthority())
			{
				ServerStopAction(Instigator, ActionName);
			}

			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}

void UYActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UYActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

void UYActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf < UYAction > Action : DefaultActions)
		{
			AddAction(GetOwner(), Action);
		}
	}
}


bool UYActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UYAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UYActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

// 	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
// 	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	for (UYAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void UYActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UYActionComponent, Actions);
}