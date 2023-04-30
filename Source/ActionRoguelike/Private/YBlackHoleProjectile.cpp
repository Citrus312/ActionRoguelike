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
	SphereComp->SetCollisionProfileName("BlackHole");//BlackHole为预设好的碰撞
	SphereComp->SetSphereRadius(64.0f);

	MovementComp->InitialSpeed = 400.0f;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(SphereComp);

	ForceComp->Radius = 800.0f;			 // 吸力范围
	ForceComp->ImpulseStrength = -100.0f; // 力
	ForceComp->bImpulseVelChange = true; // 忽略质量大小；见UE中ForceComp的“冲量速度变更”
}

// Called when the game starts or when spawned
void AYBlackHoleProjectile::BeginPlay()
{
	Super::BeginPlay();
	this->SetLifeSpan(5.0f);
}

void AYBlackHoleProjectile::PostInitializeComponents()
{
	// 执行该函数原本的功能
	Super::PostInitializeComponents();
	// 绑定到OnComponentHit事件上
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AYBlackHoleProjectile::OnComponentBeginOverlap);
}

void AYBlackHoleProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("    OtherActor is %s, at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
	if (OtherActor != GetInstigator())
	{
		UE_LOG(LogTemp, Log, TEXT("    Destroy %s, at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
		FString CombStr = FString::Printf(TEXT("Destroy %s"), *GetNameSafe(OtherActor));
		// 获取世界，位置，打印的内容，需要attach的actor，颜色，持续时间，是否有影子
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


