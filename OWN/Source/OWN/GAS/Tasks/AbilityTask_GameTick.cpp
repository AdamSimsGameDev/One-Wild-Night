// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityTask_GameTick.h"

UAbilityTask_GameTick::UAbilityTask_GameTick()
{
	bTickingTask = true;
}

UAbilityTask_GameTick* UAbilityTask_GameTick::WaitGameTick(UGameplayAbility* owningAbility)
{
	return NewAbilityTask<UAbilityTask_GameTick>(owningAbility);
}

void UAbilityTask_GameTick::TickTask(float deltaTime)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTick.Broadcast(deltaTime);
	}
}
