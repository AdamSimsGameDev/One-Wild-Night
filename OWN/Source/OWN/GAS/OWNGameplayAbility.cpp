// Fill out your copyright notice in the Description page of Project Settings.

#include "OWNGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "OWN/Input/OWNEnhancedInputComponent.h"

UOWNGameplayAbility::UOWNGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UOWNGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilitySpec& spec)
{
	Super::OnGiveAbility(actorInfo, spec);

	K2_OnGiveAbility(*actorInfo, spec);
}

void UOWNGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilitySpec& spec)
{
	Super::OnAvatarSet(actorInfo, spec);

	K2_OnAvatarSet(*actorInfo, spec);

	if (IsPassive() && CurrentActorInfo)
	{
		UpdatePassiveAbilityState();

		UAbilitySystemComponent* asc = GetAbilitySystemComponentFromActorInfo_Checked();
		PassiveTagCheckHandle = asc->RegisterGenericGameplayTagEvent().AddWeakLambda(this, [this](const FGameplayTag tag, int32 count)
			{
				this->UpdatePassiveAbilityState();
			});
	}
}

void UOWNGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilitySpec& spec)
{
	Super::OnRemoveAbility(actorInfo, spec);

	K2_OnRemoveAbility(*actorInfo, spec);

	if (IsPassive() && CurrentActorInfo)
	{
		UAbilitySystemComponent* asc = GetAbilitySystemComponentFromActorInfo();
		if (asc)
		{
			asc->RegisterGenericGameplayTagEvent().Remove(PassiveTagCheckHandle);
		}
	}
}

void UOWNGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(handle, actorInfo, activationInfo, bReplicateEndAbility, bWasCancelled);

	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().SetTimerForNextTick(this, &UOWNGameplayAbility::UpdatePassiveAbilityState);
	}
}

// TODO: Move to some shared header file with all of our nice macros
#define IS_FUNCTION_IMPLEMENTED_IN_BLUEPRINT(Obj, Function) Obj->GetClass()->IsFunctionImplementedInScript(TEXT(#Function))

bool UOWNGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, OUT FGameplayTagContainer* optionalRelevantTags) const
{
	static FGameplayTagContainer EmptyContainer;
	if (IS_FUNCTION_IMPLEMENTED_IN_BLUEPRINT(this, K2_CheckCost) && !K2_CheckCost(handle, *actorInfo, optionalRelevantTags ? *optionalRelevantTags : EmptyContainer))
	{
		return false;
	}
	return Super::CheckCost(handle, actorInfo, optionalRelevantTags);
}

void UOWNGameplayAbility::UpdatePassiveAbilityState()
{
	if (!IsActive() && CheckShouldPassiveAbilityActivate())
	{
		GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbility(CurrentSpecHandle);
	}
}

bool UOWNGameplayAbility::CheckShouldPassiveAbilityActivate()
{
	if (!CurrentActorInfo || !IsPassive() || IsActive())
	{
		return false;
	}

	// Get the owning ASC.
	UAbilitySystemComponent* asc = GetAbilitySystemComponentFromActorInfo_Checked();
	
	// Fetch the tags.
	FGameplayTagContainer tags;
	asc->GetOwnedGameplayTags(tags);

	// Check if the ability can activate.
	if (!CanActivateAbility(CurrentSpecHandle, CurrentActorInfo, &tags, &tags, nullptr))
	{
		return false;
	}

	// If we reach here, return true.
	return true;
}

APlayerController* UOWNGameplayAbility::GetPlayerControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		return CurrentActorInfo->PlayerController.Get();
	}
	return nullptr;
}

bool UOWNGameplayAbility::GetAbilityInputActionValue_Bool() const
{
	return UOWNEnhancedInputComponent::GetInputActionValue_Bool(GetPlayerControllerFromActorInfo(), InputAction);
}

float UOWNGameplayAbility::GetAbilityInputActionValue_Float() const
{
	return UOWNEnhancedInputComponent::GetInputActionValue_Float(GetPlayerControllerFromActorInfo(), InputAction);
}

FVector2D UOWNGameplayAbility::GetAbilityInputActionValue_Vector2D() const
{
	return UOWNEnhancedInputComponent::GetInputActionValue_Vector2D(GetPlayerControllerFromActorInfo(), InputAction);
}

FVector UOWNGameplayAbility::GetAbilityInputActionValue_Vector() const
{
	return UOWNEnhancedInputComponent::GetInputActionValue_Vector(GetPlayerControllerFromActorInfo(), InputAction);
}
