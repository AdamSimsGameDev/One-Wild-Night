// Fill out your copyright notice in the Description page of Project Settings.

#include "OWNPlayerController.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "OWNCharacter.h"
#include "OWN/GameFramework/OWNPlayerCameraManager.h"

AOWNPlayerController::AOWNPlayerController()
{
	WeaponStateComponent = CreateDefaultSubobject<UOWNWeaponStateComponent>(TEXT("WeaponStateComponent"));
}

AOWNPlayerCameraManager* AOWNPlayerController::GetOWNPlayerCameraManager() const
{
	return Cast<AOWNPlayerCameraManager>(PlayerCameraManager.Get());
}

void AOWNPlayerController::AcknowledgePossession(APawn* inPawn)
{
	Super::AcknowledgePossession(inPawn);

	IAbilitySystemInterface* asi = Cast<IAbilitySystemInterface>(inPawn);
	if (asi)
	{
		asi->GetAbilitySystemComponent()->InitAbilityActorInfo(inPawn, inPawn);
	}
}

void AOWNPlayerController::SetControlRotation(const FRotator& Rotator)
{
	Super::SetControlRotation(Rotator);

	if (AOWNCharacter* character = GetPawn<AOWNCharacter>())
	{
		character->ControlRotationDirection = Rotator.Vector();
	}
}
