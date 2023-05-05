// Fill out your copyright notice in the Description page of Project Settings.


#include "YActionComponent.h"
#include "YAction.h"

UYActionComponent::UYActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UYActionComponent::AddAction(AActor* Instigator, TSubclassOf<UYAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UYAction* NewAction = NewObject<UYAction>(this, ActionClass);
	if (ensure(NewAction))
	{
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
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}

void UYActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf < UYAction > Action : DefaultActions)
	{
		AddAction(GetOwner(), Action);
	}
}


void UYActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

// 	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
// 	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

