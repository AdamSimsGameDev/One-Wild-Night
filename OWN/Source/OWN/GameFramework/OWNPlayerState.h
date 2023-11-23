// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "OWN/Tags/GameplayStackTag.h"
#include "OWNPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class OWN_API AOWNPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	FGameplayTagStackContainer& GetStatTagContainer(){return PlayerStateTagStackContainer;}

private:
	UPROPERTY()
	FGameplayTagStackContainer PlayerStateTagStackContainer;

};
