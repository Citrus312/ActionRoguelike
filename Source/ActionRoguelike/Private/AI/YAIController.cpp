// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/YAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AYAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("BehaviorTree is nullptr! Please assign BehaviorTree in AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}

// 	APawn* TargetPawn = UGameplayStatics::GetPlayerPawn(this, 0);
// 	if (TargetPawn)
// 	{
// 		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", TargetPawn->GetActorLocation());
// 
// 		GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetPawn);
// 	}
}
