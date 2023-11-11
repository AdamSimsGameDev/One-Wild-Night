// Fill out your copyright notice in the Description page of Project Settings.

#include "OWNPawn.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerState.h"
#include "OWN/GAS/OWNAbilitySystemComponent.h"

// Sets default values
AOWNPawn::AOWNPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UOWNAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
}

UAbilitySystemComponent* AOWNPawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AOWNPawn::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);

	AbilitySystemComponent->InitAbilityActorInfo(GetPlayerState(), this);

	APlayerController* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		BindInputToController(pc);
	}

	LastController = pc;
}

void AOWNPawn::UnPossessed()
{
	Super::UnPossessed();

	AbilitySystemComponent->ClearActorInfo();

	if (LastController.IsValid())
	{
		UnbindInputFromController(LastController.Get());
	}
}

void AOWNPawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(GetPlayerState(), this);
}

void AOWNPawn::OnRep_Controller()
{
	Super::OnRep_Controller();

	APlayerController* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		BindInputToController(pc);
	}
	else if (LastController.IsValid())
	{
		UnbindInputFromController(LastController.Get());
	}

	LastController = pc;
}

void AOWNPawn::BindInputToController(APlayerController* controller)
{
	if (!IsValid(controller) || !controller->IsLocalController())
	{
		return;
	}

	FModifyContextOptions options;
	options.bForceImmediately = false;
	options.bIgnoreAllPressedKeysUntilRelease = true;

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer());
	if (subsystem)
	{
		subsystem->AddMappingContext(InputMappingContext, InputMappingPriority, options);
	}
}

void AOWNPawn::UnbindInputFromController(APlayerController* controller)
{
	if (!IsValid(controller) || !controller->IsLocalController())
	{
		return;
	}

	FModifyContextOptions options;
	options.bForceImmediately = false;
	options.bIgnoreAllPressedKeysUntilRelease = true;

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer());
	if (subsystem)
	{
		subsystem->RemoveMappingContext(InputMappingContext, options);
	}
}
