// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_GameTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitGameTick, float, deltaTime);

/**
 * 
 */
UCLASS()
class OWN_API UAbilityTask_GameTick : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UAbilityTask_GameTick();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Wait Game Tick", DefaultToSelf = "owningAbility", HidePin = "owningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_GameTick* WaitGameTick(UGameplayAbility* owningAbility);

protected:
	virtual void TickTask(float deltaTime) override;

public:
	UPROPERTY(BlueprintAssignable)
	FWaitGameTick OnTick;
};
