// Fill out your copyright notice in the Description page of Project Settings.


#include "YGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "AI/YAICharacter.h"
#include "YAttributeComponent.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "YCharacter.h"
#include "YPlayerState.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), false, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

AYGameModeBase::AYGameModeBase()
{
	SpawnTimerInterval = 2.0f;

	CreditsPerKill = 10;

	PlayerStateClass = AYPlayerState::StaticClass();
}

void AYGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHanlde_SpawnBots, this, &AYGameModeBase::SpawnBot_TimeElapsed, SpawnTimerInterval, true);
}

void AYGameModeBase::KillAll()
{
	for (TActorIterator<AYAICharacter> It(GetWorld()); It; ++It)
	{
		AYAICharacter* Bot = *It;
		UYAttributeComponent* BotAttributeComp = UYAttributeComponent::GetAttributes(Bot);
		if (ensure(BotAttributeComp) && BotAttributeComp->IsAlive())
		{
			BotAttributeComp->Kill(this);
		}
	}
}

void AYGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	// Respawn the Player
	AYCharacter* Player = Cast<AYCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "Respawn_TimeElapsed", Player->GetController());

		float RespawnDealy = 5.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDealy, false);
	}

	// Give Credits for kill
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		if (AYPlayerState* PS = KillerPawn->GetPlayerState<AYPlayerState>())
		{
			PS->AddCredits(CreditsPerKill);
		}
	}
}

bool AYGameModeBase::CheckBotCapicity()
{
	int32 NrOfAliveBots = 0;
	for (TActorIterator<AYAICharacter> It(GetWorld()); It; ++It)
	{
		AYAICharacter* Bot = *It;
		UYAttributeComponent* AttributeComp = UYAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), NrOfAliveBots);

	float MaxBorCnt = 10;
	if (DifficultyCurve)
	{
		MaxBorCnt = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= MaxBorCnt)
	{
		UE_LOG(LogTemp, Log, TEXT("At the maximum bot capacity, skip"));
		return false;
	}
	return true;
}

void AYGameModeBase::SpawnBot_TimeElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}

	if (!CheckBotCapicity()) return;

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AYGameModeBase::OnQueryCompleted);
	}
}

void AYGameModeBase::Respawn_TimeElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void AYGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

