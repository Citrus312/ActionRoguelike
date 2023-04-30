// Fill out your copyright notice in the Description page of Project Settings.


#include "YTargetDummy.h"
#include "YAttributeComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AYTargetDummy::AYTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UYAttributeComponent>("AttributeComp");
	AttributeComp->OnHealthChanged.AddDynamic(this, &AYTargetDummy::OnHealthChanged);
}

void AYTargetDummy::OnHealthChanged(AActor* InstigatorActor, UYAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dummy Flash"));
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}

}

