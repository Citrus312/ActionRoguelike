// Fill out your copyright notice in the Description page of Project Settings.


#include "YPlayerState.h"

int32 AYPlayerState::GetCredits() const
{
	return Credits;
}

void AYPlayerState::AddCredits(int32 Delta)
{
	if (!(ensure(Delta > 0.0f)))
	{
		return;
	}
	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool AYPlayerState::RemoveCredits(int32 Delta)
{
	if (!(ensure(Delta > 0.0f)))
	{
		return false;
	}

	if (Delta > Credits)
	{
		return false;
	}

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);

	return true;
}
