// Fill out your copyright notice in the Description page of Project Settings.

#include "OWNAbilitySystemComponent.h"
#include "InputTriggers.h"
#include "OWN/GAS/OWNGameplayAbility.h"
#include "OWN/Input/OWNEnhancedInputComponent.h"

void UOWNAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	RegisterGenericGameplayTagEvent().AddUObject(this, &UOWNAbilitySystemComponent::CheckCancelTags);
}

void UOWNAbilitySystemComponent::OnLocalAbilityInput_Internal(const UInputAction* action, ETriggerEvent triggerEvent)
{
	for (auto& spec : ActivatableAbilities.Items)
	{
		if (!spec.Ability)
		{
			continue;
		}

		const UOWNGameplayAbility* ability = CastChecked<UOWNGameplayAbility>(spec.Ability);
		if (!ability || ability->GetInputAction() != action)
		{
			continue;
		}

		spec.InputPressed = (triggerEvent == ETriggerEvent::Started || triggerEvent == ETriggerEvent::Triggered || triggerEvent == ETriggerEvent::Ongoing);
		
		if (spec.IsActive())
		{
			switch (triggerEvent)
			{
			case ETriggerEvent::Started:
			case ETriggerEvent::Triggered:
			{
				if (spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
				{
					ServerSetInputPressed(spec.Handle);
				}
				AbilitySpecInputPressed(spec);

				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, spec.Handle, spec.ActivationInfo.GetActivationPredictionKey());
				break;
			}
			case ETriggerEvent::Canceled:
			case ETriggerEvent::Completed:
			{
				if (spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
				{
					ServerSetInputReleased(spec.Handle);
				}
				AbilitySpecInputReleased(spec);

				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, spec.Handle, spec.ActivationInfo.GetActivationPredictionKey());
				break;
			}
			}
		}
		else if (ability->MatchesInputTriggerEvent(triggerEvent))
		{
			TryActivateAbility(spec.Handle);
		}
	}
}

void UOWNAbilitySystemComponent::OnLocalAbilityInputStarted(const UInputAction* action)
{
	OnLocalAbilityInput_Internal(action, ETriggerEvent::Started);
}

void UOWNAbilitySystemComponent::OnLocalAbilityInputTriggered(const UInputAction* action)
{
	OnLocalAbilityInput_Internal(action, ETriggerEvent::Triggered);
}

void UOWNAbilitySystemComponent::OnLocalAbilityInputCanceled(const UInputAction* action)
{
	OnLocalAbilityInput_Internal(action, ETriggerEvent::Canceled);
}

void UOWNAbilitySystemComponent::OnLocalAbilityInputOngoing(const UInputAction* action)
{
	OnLocalAbilityInput_Internal(action, ETriggerEvent::Ongoing);
}

void UOWNAbilitySystemComponent::OnLocalAbilityInputCompleted(const UInputAction* action)
{
	OnLocalAbilityInput_Internal(action, ETriggerEvent::Completed);
}

void UOWNAbilitySystemComponent::AddNativeActionTag(FGameplayTag GameplayTag)
{
	NativeActions.AddTag(GameplayTag);
	NativeActionCountMap.FindOrAdd(GameplayTag)++;
	UpdateTagMap(GameplayTag, 1);
	GetReplicatedLooseTags_Mutable().AddTag(GameplayTag);
}

void UOWNAbilitySystemComponent::AddNativeActionTags(FGameplayTagContainer GameplayTags)
{
	for (const FGameplayTag& Tag : GameplayTags)
	{
		AddNativeActionTag(Tag);
	}
}

void UOWNAbilitySystemComponent::RemoveNativeActionTag(FGameplayTag GameplayTag)
{
	if (!HasNativeAction(GameplayTag))
		return;

	// decrement tag count
	int32& count = NativeActionCountMap[GameplayTag];
	count--;

	// remove empty tags
	if (count == 0)
	{
		NativeActions.RemoveTag(GameplayTag);
		GetReplicatedLooseTags_Mutable().RemoveTag(GameplayTag);
	}

	UpdateTagMap(GameplayTag, -1);
}

void UOWNAbilitySystemComponent::RemoveNativeActionTags(FGameplayTagContainer GameplayTags)
{
	for (const FGameplayTag& Tag : GameplayTags)
	{
		RemoveNativeActionTag(Tag);
	}
}

void UOWNAbilitySystemComponent::OnPlayerControllerSet()
{
	Super::OnPlayerControllerSet();

	for (auto ability : ActivatableAbilities.Items)
	{
		BindAbilityInput(ability);
	}
}

void UOWNAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& abilitySpec)
{
	Super::OnGiveAbility(abilitySpec);

	BindAbilityInput(abilitySpec);
}
void UOWNAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& abilitySpec)
{
	Super::OnRemoveAbility(abilitySpec);

	UnbindAbilityInput(abilitySpec);
}

void UOWNAbilitySystemComponent::CheckCancelTags(const FGameplayTag tag, int32 count)
{
	for (auto ability : ActivatableAbilities.Items)
	{
		if (!ability.IsActive())
			continue;
		if (!CastChecked<UOWNGameplayAbility>(ability.Ability)->CancelRequiredTags.HasTag(tag))
			continue;
		CancelAbilityHandle(ability.Handle);
	}
}

void UOWNAbilitySystemComponent::BindAbilityInput(FGameplayAbilitySpec& abilitySpec)
{
	const UOWNGameplayAbility* ability = Cast<UOWNGameplayAbility>(abilitySpec.Ability);
	if (!IsValid(ability)) return; //if we're using a basic ability with no need for input stuff we can just abandon here

	const APlayerController* playerController = GetOwner<APawn>()->GetController<APlayerController>();
	if (!IsValid(playerController))
	{
		return;
	}

	UOWNEnhancedInputComponent* inputComponent = Cast<UOWNEnhancedInputComponent>(playerController->InputComponent);
	if (!inputComponent)
	{
		return;
	}

	if (!abilitySpec.Ability)
	{
		return;
	}

	
	if (ability->MatchesInputTriggerEvent(ETriggerEvent::None))
	{
		return;
	}

	if (ability->HasOptionalMappingContext())
	{
		inputComponent->AddInputMappingContext(ability->GetOptionalMappingContext(), ability->GetOptionalMappingContextPriority());
	}

	inputComponent->BindAbilityAction(abilitySpec, this, ability->GetInputAction(), ability->GetInputTriggerEvent());
	if (!ability->MatchesInputTriggerEvent(ETriggerEvent::Completed))
	{
		inputComponent->BindAbilityAction(abilitySpec, this, ability->GetInputAction(), ETriggerEvent::Completed);
	}
}

void UOWNAbilitySystemComponent::UnbindAbilityInput(FGameplayAbilitySpec& abilitySpec)
{
	const UOWNGameplayAbility* ability = Cast<UOWNGameplayAbility>(abilitySpec.Ability);
	if (!IsValid(ability)) return; //if we're using a basic ability with no need for input stuff we can just abandon here

	const APlayerController* playerController = AbilityActorInfo->PlayerController.Get();
	if (!IsValid(playerController))
	{
		return;
	}

	UOWNEnhancedInputComponent* inputComponent = Cast<UOWNEnhancedInputComponent>(playerController->InputComponent);
	if (!inputComponent)
	{
		return;
	}

	if (!abilitySpec.Ability)
	{
		return;
	}

	if (ability->MatchesInputTriggerEvent(ETriggerEvent::None))
	{
		return;
	}

	if (ability->HasOptionalMappingContext())
	{
		inputComponent->RemoveInputMappingContext(ability->GetOptionalMappingContext());
	}

	inputComponent->UnbindAbilityAction(abilitySpec.Handle, ability->GetInputAction());
}
