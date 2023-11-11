// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Pawn.h"
#include "OWNPawn.generated.h"

class UCombatSet;
class UInputMappingContext;
class UAbilitySystemComponent;
class UOWNAbilitySystemComponent;

UCLASS()
class OWN_API AOWNPawn : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AOWNPawn(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	UOWNAbilitySystemComponent* GetOWNAbilitySystemComponent() const { return AbilitySystemComponent; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void PossessedBy(AController* newController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Controller() override;

	void BindInputToController(APlayerController* controller);
	void UnbindInputFromController(APlayerController* controller);

protected:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	const UInputMappingContext* InputMappingContext = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	int32 InputMappingPriority = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UOWNAbilitySystemComponent* AbilitySystemComponent = nullptr;

private:
	// Kept around so we know what to unbind input events from.
	TWeakObjectPtr<APlayerController> LastController = nullptr;
};
