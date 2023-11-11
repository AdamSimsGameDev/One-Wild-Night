// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayAbilitySpec.h"
#include "OWNEnhancedInputComponent.generated.h"

class UInputMappingContext;
class UOWNAbilitySystemComponent;

USTRUCT()
struct OWN_API FEnhancedInputAbilityBindingInfo
{
	GENERATED_BODY()

	UPROPERTY()
	const UInputAction* Action;

	int32 StartedBinding;
	int32 TriggeredBinding;
	int32 CompletedBinding;
	int32 OngoingBinding;
	int32 CanceledBinding;

	FEnhancedInputActionValueBinding ValueBinding;

	int32 InputID;

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> BoundAbilities;

	FEnhancedInputAbilityBindingInfo() { Action = nullptr; }
	FEnhancedInputAbilityBindingInfo(const UInputAction* action)
	{
		Action = action;
	}

	bool operator==(const FEnhancedInputAbilityBindingInfo& info) const { return info.Action == Action; }
	bool operator==(const UInputAction* action) const { return action == Action; }
};

/**
 * 
 */
UCLASS()
class OWN_API UOWNEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	void BindAbilityActions(FGameplayAbilitySpec& spec, UOWNAbilitySystemComponent* asc, const TArray<const UInputAction*>& actions, ETriggerEvent triggerEvent);
	void UnbindAbilityActions(FGameplayAbilitySpecHandle handle, const TArray<const UInputAction*>& actions);

	void BindAbilityAction(FGameplayAbilitySpec& spec, UOWNAbilitySystemComponent* asc, const UInputAction* action, ETriggerEvent triggerEvent);
	void UnbindAbilityAction(FGameplayAbilitySpecHandle handle, const UInputAction* action);

	void AddInputMappingContext(const UInputMappingContext* imc, int32 priority);
	void RemoveInputMappingContext(const UInputMappingContext* imc);

	ULocalPlayer* GetLocalPlayer() const;

	UFUNCTION(BlueprintCallable, Category = Input, meta = (DisplayName = "Get Input Action Value (bool)"))
	static bool GetInputActionValue_Bool(APlayerController* controller, const UInputAction* action);
	UFUNCTION(BlueprintCallable, Category = Input, meta = (DisplayName = "Get Input Action Value (float)"))
	static float GetInputActionValue_Float(APlayerController* controller, const UInputAction* action);
	UFUNCTION(BlueprintCallable, Category = Input, meta = (DisplayName = "Get Input Action Value (Vector2D)"))
	static FVector2D GetInputActionValue_Vector2D(APlayerController* controller, const UInputAction* action);
	UFUNCTION(BlueprintCallable, Category = Input, meta = (DisplayName = "Get Input Action Value (Vector)"))
	static FVector GetInputActionValue_Vector(APlayerController* controller, const UInputAction* action);

protected:

	void OnAbilityActionStarted(const UInputAction* action, UOWNAbilitySystemComponent* asc);
	void OnAbilityActionTriggered(const UInputAction* action, UOWNAbilitySystemComponent* asc);
	void OnAbilityActionCanceled(const UInputAction* action, UOWNAbilitySystemComponent* asc);
	void OnAbilityActionOngoing(const UInputAction* action, UOWNAbilitySystemComponent* asc);
	void OnAbilityActionCompleted(const UInputAction* action, UOWNAbilitySystemComponent* asc);

protected:
	UPROPERTY()
	TArray<FEnhancedInputAbilityBindingInfo> BoundAbilities;

private:
	static int32 GetNextInputID();
	static int32 NextInputID;
};
