// Fill out your copyright notice in the Description page of Project Settings.

#include "OWNEnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "OWN/GAS/OWNAbilitySystemComponent.h"

int32 UOWNEnhancedInputComponent::NextInputID = 0;

void UOWNEnhancedInputComponent::BindAbilityActions(FGameplayAbilitySpec& spec, UOWNAbilitySystemComponent* asc, const TArray<const UInputAction*>& actions, ETriggerEvent triggerEvent)
{
    for (const auto& action : actions)
    {
        BindAbilityAction(spec, asc, action, triggerEvent);
    }
}

void UOWNEnhancedInputComponent::UnbindAbilityActions(FGameplayAbilitySpecHandle handle, const TArray<const UInputAction*>& actions)
{
    for (const auto& action : actions)
    {
        UnbindAbilityAction(handle, action);
    }
}

void UOWNEnhancedInputComponent::BindAbilityAction(FGameplayAbilitySpec& spec, UOWNAbilitySystemComponent* asc, const UInputAction* action, ETriggerEvent triggerEvent)
{
    FEnhancedInputAbilityBindingInfo* binding = BoundAbilities.FindByKey(action);
    if (!binding)
    {
        binding = &BoundAbilities.Emplace_GetRef(action);
        binding->InputID = GetNextInputID();
    }

    binding->BoundAbilities.AddUnique(spec.Handle);
    binding->ValueBinding = BindActionValue(action);

    switch (triggerEvent)
    {
    case ETriggerEvent::Triggered:
        binding->TriggeredBinding = BindAction(action, triggerEvent, this, &UOWNEnhancedInputComponent::OnAbilityActionTriggered, action, asc).GetHandle();
        break;
    case ETriggerEvent::Started:
        binding->StartedBinding = BindAction(action, triggerEvent, this, &UOWNEnhancedInputComponent::OnAbilityActionStarted, action, asc).GetHandle();
        break;
    case ETriggerEvent::Ongoing:
        binding->OngoingBinding = BindAction(action, triggerEvent, this, &UOWNEnhancedInputComponent::OnAbilityActionOngoing, action, asc).GetHandle();
        break;
    case ETriggerEvent::Canceled:
        binding->CanceledBinding = BindAction(action, triggerEvent, this, &UOWNEnhancedInputComponent::OnAbilityActionCanceled, action, asc).GetHandle();
        break;
    case ETriggerEvent::Completed:
        binding->CompletedBinding = BindAction(action, triggerEvent, this, &UOWNEnhancedInputComponent::OnAbilityActionCompleted, action, asc).GetHandle();
        break;
    }

    spec.InputID = binding->InputID;
}

void UOWNEnhancedInputComponent::UnbindAbilityAction(FGameplayAbilitySpecHandle handle, const UInputAction* action)
{
    FEnhancedInputAbilityBindingInfo* binding = BoundAbilities.FindByKey(action);
    if (!binding)
    {
        return;
    }
    binding->BoundAbilities.Remove(handle);
}

void UOWNEnhancedInputComponent::AddInputMappingContext(const UInputMappingContext* imc, int32 priority)
{
    UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    check(subsystem);

    FModifyContextOptions options;
    options.bForceImmediately = false;
    options.bIgnoreAllPressedKeysUntilRelease = true;
    subsystem->AddMappingContext(imc, priority, options);
}

void UOWNEnhancedInputComponent::RemoveInputMappingContext(const UInputMappingContext* imc)
{
    UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    check(subsystem);

    FModifyContextOptions options;
    options.bForceImmediately = false;
    options.bIgnoreAllPressedKeysUntilRelease = true;
    subsystem->RemoveMappingContext(imc, options);
}

ULocalPlayer* UOWNEnhancedInputComponent::GetLocalPlayer() const
{
    return GetOwner<APlayerController>()->GetLocalPlayer();
}

bool UOWNEnhancedInputComponent::GetInputActionValue_Bool(APlayerController* controller, const UInputAction* action)
{
    if (UEnhancedInputComponent* eic = Cast<UEnhancedInputComponent>(controller->InputComponent.Get()))
    {
        return eic->GetBoundActionValue(action).Get<bool>();
    }
    return false;
}

float UOWNEnhancedInputComponent::GetInputActionValue_Float(APlayerController* controller, const UInputAction* action)
{
    if (UEnhancedInputComponent* eic = Cast<UEnhancedInputComponent>(controller->InputComponent.Get()))
    {
        return eic->GetBoundActionValue(action).Get<float>();
    }
    return 0.0f;
}

FVector2D UOWNEnhancedInputComponent::GetInputActionValue_Vector2D(APlayerController* controller, const UInputAction* action)
{
    if (UEnhancedInputComponent* eic = Cast<UEnhancedInputComponent>(controller->InputComponent.Get()))
    {
        return eic->GetBoundActionValue(action).Get<FVector2D>();
    }
    return FVector2D::ZeroVector;
}

FVector UOWNEnhancedInputComponent::GetInputActionValue_Vector(APlayerController* controller, const UInputAction* action)
{
    if (UEnhancedInputComponent* eic = Cast<UEnhancedInputComponent>(controller->InputComponent.Get()))
    {
        return eic->GetBoundActionValue(action).Get<FVector>();
    }
    return FVector::ZeroVector;
}

#define ONABILITYACTIONRESPONSE(Type)                                                   \
{                                                                                       \
    FEnhancedInputAbilityBindingInfo* binding = BoundAbilities.FindByKey(action);       \
    if (!binding)                                                                       \
    {                                                                                   \
        return;                                                                         \
    }                                                                                   \
    if (!asc)                                                                           \
    {                                                                                   \
        return;                                                                         \
    }                                                                                   \
    asc->OnLocalAbilityInput##Type(action);                                             \
}                                                                                       \

void UOWNEnhancedInputComponent::OnAbilityActionStarted(const UInputAction* action, UOWNAbilitySystemComponent* asc)
{
    ONABILITYACTIONRESPONSE(Started);
}

void UOWNEnhancedInputComponent::OnAbilityActionTriggered(const UInputAction* action, UOWNAbilitySystemComponent* asc)
{
    ONABILITYACTIONRESPONSE(Triggered);
}

void UOWNEnhancedInputComponent::OnAbilityActionCanceled(const UInputAction* action, UOWNAbilitySystemComponent* asc)
{
    ONABILITYACTIONRESPONSE(Canceled);
}

void UOWNEnhancedInputComponent::OnAbilityActionOngoing(const UInputAction* action, UOWNAbilitySystemComponent* asc)
{
    ONABILITYACTIONRESPONSE(Ongoing);
}

void UOWNEnhancedInputComponent::OnAbilityActionCompleted(const UInputAction* action, UOWNAbilitySystemComponent* asc)
{
    ONABILITYACTIONRESPONSE(Completed);
}

int32 UOWNEnhancedInputComponent::GetNextInputID()
{
    return NextInputID++;
}
