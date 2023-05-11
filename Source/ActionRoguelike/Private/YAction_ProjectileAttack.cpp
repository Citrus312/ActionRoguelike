// Fill out your copyright notice in the Description page of Project Settings.


#include "YAction_ProjectileAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

UYAction_ProjectileAttack::UYAction_ProjectileAttack()
{
	AttackAnimDelay = 0.2f;
	AttackDistance = 2000.0f;

	SpawnLocationName = "Muzzle_01";
}

void UYAction_ProjectileAttack::GetAttackTM(ACharacter* Instigator, FTransform& SpawnTM)
{
	FVector HandLocation = Instigator->GetMesh()->GetSocketLocation(SpawnLocationName);
	FVector CameraLocation = Instigator->GetPawnViewLocation();
	FVector TargetLocation = CameraLocation + (Instigator->GetControlRotation().Vector() * AttackDistance);

	//��⾲̬����
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	//��Ŀ�귽����й���׷�٣��������
	FHitResult Hit;
	FRotator AttackRotation;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, TargetLocation, ObjectQueryParams);

	if (bBlockingHit)
	{
		//�������壬�����õ㷽����й���
		UE_LOG(LogTemp, Log, TEXT("The attack target found is %s, at game time %f"), *GetNameSafe(Hit.GetActor()), GetWorld()->TimeSeconds);
		FVector HitPoint = Hit.ImpactPoint;
		AttackRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, HitPoint);
	}
	else
	{
		//�������壬��Ĭ�Ϸ�����й���
		UE_LOG(LogTemp, Log, TEXT("There is nothing found."));
		AttackRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TargetLocation);
	}

	SpawnTM = FTransform(AttackRotation, HandLocation);
}

void UYAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensure(ProjectileClass))
	{
		//������Ч
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, InstigatorCharacter->GetMesh(), SpawnLocationName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		//����
		FTransform SpawnTM;
		GetAttackTM(InstigatorCharacter, SpawnTM);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}

void UYAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);

		if (Character->HasAuthority())
		{
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
		}
	}
}
