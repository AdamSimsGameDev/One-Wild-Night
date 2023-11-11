// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OWNPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OWN_API AOWNPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void AcknowledgePossession(APawn* inPawn) override;
};
