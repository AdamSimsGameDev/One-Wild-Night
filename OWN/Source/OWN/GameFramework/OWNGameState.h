// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OWN/Tags/GameplayStackTag.h"
#include "OWNGameState.generated.h"

/**
 * 
 */
UCLASS()
class OWN_API AOWNGameState : public AGameStateBase
{
	GENERATED_BODY()


private:

	UPROPERTY()
	FGameplayTagStackContainer GameStateTagStackContainer;
};
