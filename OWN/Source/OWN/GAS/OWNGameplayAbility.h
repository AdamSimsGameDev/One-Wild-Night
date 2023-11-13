// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "OWNGameplayAbility.generated.h"

enum class ETriggerEvent : uint8;
class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class OWN_API UOWNGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UOWNGameplayAbility();

	bool IsPassive() const { return bIsPassive; }

	const UInputAction* GetInputAction() const { return InputAction; }
	ETriggerEvent GetInputTriggerEvent() const { return InputTriggerEvent; }

	bool HasOptionalMappingContext() const { return bBindOptionalMappingContext && OptionalMappingContext; }
	const UInputMappingContext* GetOptionalMappingContext() const { return OptionalMappingContext; }
	int32 GetOptionalMappingContextPriority() const { return OptionalMappingContextPriority; }

	UFUNCTION(BlueprintCallable)
	bool MatchesInputTriggerEvent(ETriggerEvent inType) const { return inType == InputTriggerEvent; }

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilitySpec& spec) override;
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilitySpec& spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilitySpec& spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, OUT FGameplayTagContainer* optionalRelevantTags = nullptr) const;
	virtual void UpdatePassiveAbilityState();
	virtual bool CheckShouldPassiveAbilityActivate();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Give Ability"))
	void K2_OnGiveAbility(const FGameplayAbilityActorInfo& actorInfo, const FGameplayAbilitySpec& spec);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Avatar Set"))
	void K2_OnAvatarSet(const FGameplayAbilityActorInfo& actorInfo, const FGameplayAbilitySpec& spec);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Remove Ability"))
	void K2_OnRemoveAbility(const FGameplayAbilityActorInfo& actorInfo, const FGameplayAbilitySpec& spec);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Check Cost"))
	bool K2_CheckCost(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo& actorInfo, FGameplayTagContainer& optionalRelevantTags) const;

	UFUNCTION(BlueprintCallable)
	APlayerController* GetPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = Input, meta = (DisplayName = "Get Input Action Value (bool)"))
	bool GetAbilityInputActionValue_Bool() const;
	UFUNCTION(BlueprintCallable, Category = Input, meta = (DisplayName = "Get Input Action Value (float)"))
	float GetAbilityInputActionValue_Float() const;
	UFUNCTION(BlueprintCallable, Category = Input, meta = (DisplayName = "Get Input Action Value (Vector2D)"))
	FVector2D GetAbilityInputActionValue_Vector2D() const;
	UFUNCTION(BlueprintCallable, Category = Input, meta = (DisplayName = "Get Input Action Value (Vector)"))
	FVector GetAbilityInputActionValue_Vector() const;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Tags)
	FGameplayTagContainer CancelRequiredTags;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Ability)
	bool bIsPassive;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Input)
	ETriggerEvent InputTriggerEvent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Input)
	const UInputAction* InputAction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Input, meta = (InlineEditConditionToggle))
	bool bBindOptionalMappingContext;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Input, meta = (EditCondition = "bBindOptionalMappingContext"))
	const UInputMappingContext* OptionalMappingContext;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Input, meta = (EditCondition = "bBindOptionalMappingContext"))
	int32 OptionalMappingContextPriority;

private:
	FDelegateHandle PassiveTagCheckHandle;
};
