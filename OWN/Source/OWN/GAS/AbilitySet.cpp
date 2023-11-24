// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySet.h"
#include "OWNAbilitySystemComponent.h"
#include "OWNGameplayAbility.h"
#include "OWN/GameFramework/OWNPlayerController.h"

FAbilitySetHandles UAbilitySet::AddAbilitySet(const UAbilitySet* abilitySet, UOWNAbilitySystemComponent* abilitySystemComponent, UObject* sourceObject)
{
	if (!abilitySystemComponent || !abilitySet)
	{
		return FAbilitySetHandles();
	}

	FAbilitySetHandles handles;
	handles.GrantedSet = abilitySet;

	for (const FAbilitySetAbilityData& ability : abilitySet->Abilities)
	{
		FGameplayAbilitySpec abilitySpec = abilitySystemComponent->BuildAbilitySpecFromClass(ability.AbilityClass, ability.AbilityLevel, -1);
		abilitySpec.SourceObject = sourceObject;
		handles.AbilityHandles.Add(abilitySystemComponent->GiveAbility(abilitySpec));
	}

	for (const TSubclassOf<UGameplayEffect>& effect : abilitySet->Effects)
	{
		handles.EffectHandles.Add(abilitySystemComponent->ApplyGameplayEffectToSelf(effect.GetDefaultObject(), 0.0f, abilitySystemComponent->MakeEffectContext()));
	}

	return handles;
}

void UAbilitySet::RemoveAbilitySet(UOWNAbilitySystemComponent* abilitySystemComponent, FAbilitySetHandles handles)
{
	if (!abilitySystemComponent || !handles.GrantedSet)
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& handle : handles.AbilityHandles)
	{
		abilitySystemComponent->ClearAbility(handle);
	}

	for (const FActiveGameplayEffectHandle& handle : handles.EffectHandles)
	{
		abilitySystemComponent->RemoveActiveGameplayEffect(handle);
	}
}
