// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/YAICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "YWorldUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "YActionComponent.h"

// Sets default values
AYAICharacter::AYAICharacter()
{
    AttributeComp = CreateDefaultSubobject<UYAttributeComponent>("AttributeComp");

    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

    ActionComp = CreateDefaultSubobject<UYActionComponent>("ActionComp");

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
    GetMesh()->SetGenerateOverlapEvents(true);

    TimeToHitParaName = "TimeToHit";
}

void AYAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    PawnSensingComp->OnSeePawn.AddDynamic(this, &AYAICharacter::OnPawnSeen);
    AttributeComp->OnHealthChanged.AddDynamic(this, &AYAICharacter::OnHealthChanged);
}

void AYAICharacter::OnHealthChanged(AActor* InstigatorActor, UYAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    if (Delta < 0.0f)
    {
        if (InstigatorActor != this)
        {
            SetTargetActor(InstigatorActor);
        }

        //HealthBar
        ShowHealthBar();
        
        //flash
        GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParaName, GetWorld()->TimeSeconds);

        //Die
        if (NewHealth <= 0.0f)
        {
            AAIController* AIC = Cast<AAIController>(GetController());
            if (AIC)
            {
                AIC->GetBrainComponent()->StopLogic("Killed");
            }

            //Ragdoll
            GetMesh()->SetAllBodiesSimulatePhysics(true);
            GetMesh()->SetCollisionProfileName("RagDoll");

            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            GetCharacterMovement()->DisableMovement();

            //Set life span
            SetLifeSpan(5.0f);
        }
    }
}

void AYAICharacter::SetTargetActor(AActor* NewTarget)
{
    AAIController* AIC = Cast<AAIController>(GetController());
    if (ensure(AIC))
    {
        if (AIC->GetBlackboardComponent()->GetValueAsObject("TargetActor") == nullptr)
        {
            ShowPlayerSpottedWidget();
        }
        AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);

    }
}

void AYAICharacter::ShowHealthBar()
{
    if (ActiveHealthBar == nullptr)
    {
        ActiveHealthBar = CreateWidget<UYWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
        if (ActiveHealthBar)
        {
            ActiveHealthBar->AttachedActor = this;
            ActiveHealthBar->AddToViewport();
        }
    }
}

void AYAICharacter::ShowPlayerSpottedWidget()
{
    if (PlayerSpottedWidget == nullptr)
    {
        PlayerSpottedWidget = CreateWidget<UYWorldUserWidget>(GetWorld(), SpottedWidgetClass);
        if (PlayerSpottedWidget)
        {
            PlayerSpottedWidget->AttachedActor = this;
            PlayerSpottedWidget->AddToViewport(10);
            FTimerHandle TimerHanlde_PlayerSpotted;
            GetWorld()->GetTimerManager().SetTimer(TimerHanlde_PlayerSpotted, this, &AYAICharacter::RemovePlayerSpottedWidget, 2.0f);
        }
    }
}

void AYAICharacter::RemovePlayerSpottedWidget()
{
    if (PlayerSpottedWidget)
    {
        PlayerSpottedWidget->RemoveFromViewport();
    }
}

void AYAICharacter::OnPawnSeen(APawn* Pawn)
{
    SetTargetActor(Pawn);
}


