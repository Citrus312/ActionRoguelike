// Fill out your copyright notice in the Description page of Project Settings.


#include "YInteractionComponent.h"
#include "YGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "YWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), true, TEXT("Enable drawing debug of interaction."), ECVF_Cheat);

// Sets default values for this component's properties
UYInteractionComponent::UYInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 400.0f;
	TraceRadius = 30.0f;
	CollisionChannel = ECC_WorldDynamic;
}


void UYInteractionComponent::FindBestInteract()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() *= TraceDistance);

	// 	FHitResult Hit;
	// 	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult>Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Yellow;


	FocusActor = nullptr;
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			if (HitActor->Implements<UYGameplayInterface>())
			{
				FocusActor = HitActor;
				break;
			}
		}
	}

	if (FocusActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UYWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 0.0f, 0, 2.0f);
	}
}

void UYInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());

	IYGameplayInterface::Execute_Interact(InFocus, MyPawn);
}

// Called when the game starts
void UYInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UYInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteract();
	}
}

void UYInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusActor);
}

