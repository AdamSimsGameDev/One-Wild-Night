// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "OWNAbilitySystemGlobals.generated.h"

class UOWNAbilitySystemComponent;

UCLASS()
class OWN_API UOWNAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;

	UFUNCTION(BlueprintCallable, Category = OWN)
	static UOWNAbilitySystemComponent* GetOWNAbilitySystemComponentFromActor(AActor* actor);
};
