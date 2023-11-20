// Fill out your copyright notice in the Description page of Project Settings.

#include "OWNAbilitySystemGlobals.h"
#include "OWNGameplayEffectContext.h"
#include "OWNAbilitySystemComponent.h"

FGameplayEffectContext* UOWNAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FOWNGameplayEffectContext();
}

UOWNAbilitySystemComponent* UOWNAbilitySystemGlobals::GetOWNAbilitySystemComponentFromActor(AActor* actor)
{
	return Cast<UOWNAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(actor));
}
