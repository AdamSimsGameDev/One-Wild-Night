// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OWNVerbMessageHelpers.generated.h"

struct FGameplayCueParameters;
struct FOWNVerbMessage;

class APlayerController;
class APlayerState;
class UObject;
struct FFrame;

/**
 * 
 */
UCLASS()
class OWN_API UOWNVerbMessageHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "OWN")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "OWN")
	static APlayerController* GetPlayerControllerFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "OWN")
	static FGameplayCueParameters VerbMessageToCueParameters(const FOWNVerbMessage& Message);

	UFUNCTION(BlueprintCallable, Category = "OWN")
	static FOWNVerbMessage CueParametersToVerbMessage(const FGameplayCueParameters& Params);	
};
