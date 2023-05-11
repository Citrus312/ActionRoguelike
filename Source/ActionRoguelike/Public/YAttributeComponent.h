// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/UnrealMathUtility.h"
#include "YAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, UYAttributeComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UYAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UYAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UYAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* FromActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float HealthMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float RageMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float RageConvertRatio;

	// @FIXME: mark as unreliable once we moved the 'state' our of YCharacter
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

public:	
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	bool AddRage(float Delta);

	UFUNCTION(BlueprintCallable)
	bool IsRageMax() const;

	UFUNCTION(BlueprintCallable)
	void ClearRage();

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool ApplyHealthChange(AActor* Instigator, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool ApplyRageChange(AActor* Instigator,float Delta);

};
