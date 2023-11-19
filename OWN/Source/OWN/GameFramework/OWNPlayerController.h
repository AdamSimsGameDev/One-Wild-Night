// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/OWNWeaponStateComponent.h"
#include "GameFramework/PlayerController.h"
#include "OWNPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OWN_API AOWNPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AOWNPlayerController();

	UFUNCTION(BlueprintCallable)
	class AOWNPlayerCameraManager* GetOWNPlayerCameraManager() const;

protected:
	virtual void AcknowledgePossession(APawn* inPawn) override;
	virtual void SetControlRotation(const FRotator& Rotator) override;

public:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UOWNWeaponStateComponent* WeaponStateComponent = nullptr;
};
