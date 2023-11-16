// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "OWN/GAS/AbilitySet.h"
#include "Logging/LogMacros.h"
#include "Component/OWNHealthComponent.h"
#include "OWNCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class UOWNAbilitySystemComponent;
class UAbilitySystemComponent;

class UCharacterDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterDefinitionSet, class AOWNCharacter*, character, UCharacterDefinition*, definition);

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AOWNCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
public:
	AOWNCharacter();
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	UOWNAbilitySystemComponent* GetOWNAbilitySystemComponent() const { return AbilitySystemComponent; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* newController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Controller() override;

	UFUNCTION(BlueprintCallable)
	void Move(FVector2D Value);
	UFUNCTION(BlueprintCallable)
	void Look(FVector2D Value);
			
	void BindInputToController(APlayerController* controller);
	void UnbindInputFromController(APlayerController* controller);

	UFUNCTION()
	void OnRep_CharacterDefinition();

	UFUNCTION(Server, Reliable)
	void ServerSetCharacterDefinition(UCharacterDefinition* definition);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Character Definition Set"))
	void K2_OnCharacterDefinitionSet(UCharacterDefinition* definition);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	void SetCharacterDefinition(UCharacterDefinition* definition);

	UPROPERTY(BlueprintReadOnly, Replicated)
	FVector_NetQuantizeNormal ControlRotationDirection;

	UPROPERTY(BlueprintAssignable)
	FOnCharacterDefinitionSet OnCharacterDefinitionSet;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	const UInputMappingContext* InputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	int32 InputMappingPriority = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UOWNAbilitySystemComponent* AbilitySystemComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UOWNHealthComponent* HealthComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CharacterDefinition)
	UCharacterDefinition* CharacterDefinition = nullptr;

	FAbilitySetHandles AbilitySetHandles;

private:
	// Kept around so we know what to unbind input events from.
	TWeakObjectPtr<APlayerController> LastController = nullptr;
};

