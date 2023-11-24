// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayAbilitySpec.h"
#include "Engine/DataAsset.h"
#include "AbilitySet.generated.h"

class UOWNAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FAbilitySetAbilityData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class UOWNGameplayAbility> AbilityClass;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 AbilityLevel;
};

USTRUCT(BlueprintType)
struct FAbilitySetHandles
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	const UAbilitySet* GrantedSet;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FGameplayAbilitySpecHandle> AbilityHandles;

	UPROPERTY(BlueprintReadOnly)
	TArray<FActiveGameplayEffectHandle> EffectHandles;
};

UCLASS(Blueprintable)
class OWN_API UAbilitySet : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AbilitySet)
	TArray<FAbilitySetAbilityData> Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AbilitySet)
	TArray<TSubclassOf<class UGameplayEffect>> Effects;

	UFUNCTION(BlueprintCallable, Category = AbilitySet)
	static FAbilitySetHandles AddAbilitySet(const UAbilitySet* abilitySet, UOWNAbilitySystemComponent* abilitySystemComponent, UObject* sourceObject);
	
	UFUNCTION(BlueprintCallable, Category = AbilitySet)
	static void RemoveAbilitySet(UOWNAbilitySystemComponent* abilitySystemComponent, FAbilitySetHandles handles);
};
