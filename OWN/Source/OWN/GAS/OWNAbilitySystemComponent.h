// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "OWNAbilitySystemComponent.generated.h"

enum class ETriggerEvent : uint8;
class UInputAction;

/**
 * 
 */
UCLASS()
class OWN_API UOWNAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void OnLocalAbilityInputStarted(const UInputAction* action);
	void OnLocalAbilityInputTriggered(const UInputAction* action);
	void OnLocalAbilityInputCanceled(const UInputAction* action);
	void OnLocalAbilityInputOngoing(const UInputAction* action);
	void OnLocalAbilityInputCompleted(const UInputAction* action);

	// Native Actions

	UFUNCTION(BlueprintCallable, Category = NativeActions)
	void AddNativeActionTag(FGameplayTag GameplayTag);

	UFUNCTION(BlueprintCallable, Category = NativeActions)
	void AddNativeActionTags(FGameplayTagContainer GameplayTags);

	UFUNCTION(BlueprintCallable, Category = NativeActions)
	void RemoveNativeActionTag(FGameplayTag GameplayTag);

	UFUNCTION(BlueprintCallable, Category = NativeActions)
	void RemoveNativeActionTags(FGameplayTagContainer GameplayTags);

	UFUNCTION(BlueprintCallable, Category = NativeActions)
	bool HasNativeAction(FGameplayTag GameplayTag) const
	{
		return NativeActions.HasTag(GameplayTag);
	}	
	UFUNCTION(BlueprintCallable, Category = NativeActions)
	bool HasNativeActions(FGameplayTagContainer GameplayTags) const
	{
		return NativeActions.HasAll(GameplayTags);
	}

protected:
	virtual void OnPlayerControllerSet() override;
	virtual void OnGiveAbility(FGameplayAbilitySpec& abilitySpec) override;
	virtual void OnRemoveAbility(FGameplayAbilitySpec& abilitySpec) override;

	void CheckCancelTags(const FGameplayTag tag, int32 count);

	void BindAbilityInput(FGameplayAbilitySpec& abilitySpec);
	void UnbindAbilityInput(FGameplayAbilitySpec& abilitySpec);

	void OnLocalAbilityInput_Internal(const UInputAction* action, ETriggerEvent triggerEvent);

protected:
	FGameplayTagContainer NativeActions;
	TMap<FGameplayTag, int32> NativeActionCountMap;
};