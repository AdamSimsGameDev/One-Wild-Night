// Fill out your copyright notice in the Description page of Project Settings.

#include "OWNAbilitySystemGlobals.h"
#include "OWNGameplayEffectContext.h"

FGameplayEffectContext* UOWNAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FOWNGameplayEffectContext();
}
