// Fill out your copyright notice in the Description page of Project Settings.


#include "YBlackHoleProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework//ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Engine/World.h"


// Sets default values
AYBlackHoleProjectile::AYBlackHoleProjectile()
{
	SphereComp->SetCollisionProfileName("BlackHole");//BlackHoleΪԤ��õ���ײ
	SphereComp->SetSphereRadius(64.0f);

	MovementComp->InitialSpeed = 400.0f;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(SphereComp);

	ForceComp->Radius = 800.0f;			 // ������Χ
	ForceComp->ImpulseStrength = -100.0f; // ��
	ForceComp->bImpulseVelChange = true; // ����������С����UE��ForceComp�ġ������ٶȱ����
}

// Called when the game starts or when spawned
void AYBlackHoleProjectile::BeginPlay()
{
	Super::BeginPlay();
	this->SetLifeSpan(5.0f);
}

void AYBlackHoleProjectile::PostInitializeComponents()
{
	// ִ�иú���ԭ���Ĺ���
	Super::PostInitializeComponents();
	// �󶨵�OnComponentHit�¼���
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AYBlackHoleProjectile::OnComponentBeginOverlap);
}

void AYBlackHoleProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("    OtherActor is %s, at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
	if (OtherActor != GetInstigator())
	{
		UE_LOG(LogTemp, Log, TEXT("    Destroy %s, at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
		FString CombStr = FString::Printf(TEXT("Destroy %s"), *GetNameSafe(OtherActor));
		// ��ȡ���磬λ�ã���ӡ�����ݣ���Ҫattach��actor����ɫ������ʱ�䣬�Ƿ���Ӱ��
		DrawDebugString(GetWorld(), OtherActor->GetActorLocation(), CombStr, nullptr, FColor::Green, 2.0f, true);
		OtherActor->Destroy();
	}
}

// Called every frame
void AYBlackHoleProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ForceComp->FireImpulse();
}


