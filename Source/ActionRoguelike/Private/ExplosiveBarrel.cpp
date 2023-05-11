// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionProfileName("PhysicsActor");
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);

	ForceComp->Radius = 500.0f;			 // ��ը��Χ
	ForceComp->ImpulseStrength = 700.0f; // �����
	ForceComp->bImpulseVelChange = true; // ����������С����UE��ForceComp�ġ������ٶȱ����

}

// PostInitializeComponents��Actor��ʼ����Ϻ��ٵ���
void AExplosiveBarrel::PostInitializeComponents()
{
	// ִ�иú���ԭ���Ĺ���
	Super::PostInitializeComponents();
	// �󶨵�OnComponentHit�¼���
	MeshComp->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::OnActorHit);
}


void AExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();

	// log��Ϣ��category��log/warning/error�ȱ�ʾ��־����ϸ�̶ȣ���ӡ����������

	UE_LOG(LogTemp, Log, TEXT("OtherActor is %s, at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
	UE_LOG(LogTemp, Warning, TEXT("HHHHHHHHHHHHH"));

	FString CombStr = FString::Printf(TEXT("Hit at %s"), *Hit.ImpactPoint.ToString());
	// ��ȡ���磬λ�ã���ӡ�����ݣ���Ҫattach��actor����ɫ������ʱ�䣬�Ƿ���Ӱ��
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombStr, nullptr, FColor::Green, 2.0f, true);
}

void AExplosiveBarrel::Explode()
{
	ForceComp->FireImpulse();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	SetLifeSpan(2.0f);
}
