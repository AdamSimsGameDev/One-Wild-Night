// Fill out your copyright notice in the Description page of Project Settings.

#include "OWNPlayerController.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

void AOWNPlayerController::AcknowledgePossession(APawn* inPawn)
{
	Super::AcknowledgePossession(inPawn);

	IAbilitySystemInterface* asi = Cast<IAbilitySystemInterface>(inPawn);
	if (asi)
	{
		asi->GetAbilitySystemComponent()->InitAbilityActorInfo(inPawn, inPawn);
	}
}