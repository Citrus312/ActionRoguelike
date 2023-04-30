// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "YCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UParticleSystem;
class UYInteractionComponent;
class UAnimMontage;
class UYAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AYCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> AdvancedProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* PrimaryAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AdvancedAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* DashAnim;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	FName SpawnLocationName_PrimaryAttack;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	FName SpawnLocationName_AdvancedAttack;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	FName SpawnLocationName_Dash;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_AdvancedAttack;
	FTimerHandle TimerHandle_Dash;

public:
	// Sets default values for this character's properties
	AYCharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UYInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UYAttributeComponent* AttributeComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void GetAttackTM(FTransform& SpawnTM, FName SpawnLocationName);
	void SpawnAttackProjectile(TSubclassOf<AActor> ProjectileClass, FName SpawnLocationName);
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();
	void AdvancedAttack();
	void AdvancedAttack_TimeElapsed();
	void Dash();
	void Dash_TimeElapsed();

	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UYAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

	virtual FVector GetPawnViewLocation() const override;

	float AttackDistance = 2000.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

};
