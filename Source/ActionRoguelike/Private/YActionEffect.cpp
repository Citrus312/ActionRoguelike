// Fill out your copyright notice in the Description page of Project Settings.


#include "YActionEffect.h"
#include "../Public/YActionComponent.h"

UYActionEffect::UYActionEffect()
{
	bAutoStart = true;
}

void UYActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(TimerHanlde_Duration, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(TimerHanlde_Period, Delegate, Period, true);
	}

}

void UYActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(TimerHanlde_Period) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(TimerHanlde_Duration);
	GetWorld()->GetTimerManager().ClearTimer(TimerHanlde_Period);

	UYActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}

void UYActionEffect::ExecutePeriodEffect_Implementation(AActor* Instigator)
{
	FString DebugMsg = "Action Effect " + GetNameSafe(this) + " execute!";
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White,  DebugMsg);
}
