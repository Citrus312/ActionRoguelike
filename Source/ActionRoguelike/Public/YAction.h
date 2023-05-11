// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "YAction.generated.h"

class UWorld;


USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UYAction : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(Replicated)
	UYActionComponent* ActionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockTags;

	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();

	UFUNCTION(BlueprintCallable, Category = "Action")
	UYActionComponent* GetOwningComponent() const;

public:

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart = false;

	void Initialize(UYActionComponent* NewActionComp);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StopAction(AActor* Instigator);

	//nick name to start/stop without a reference to the object
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UWorld* GetWorld()const override;
	
	bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
