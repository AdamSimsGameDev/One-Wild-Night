// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "OWNPlayerCameraManager.generated.h"

class USpringArmComponent;

USTRUCT()
struct FSpringArmProperties
{
	GENERATED_BODY()

	float TargetArmLength;
	FVector SocketOffset;
	FVector TargetOffset;
	FVector RelativeLocation;
	FRotator RelativeRotation;
};

/**
 * 
 */
UCLASS()
class OWN_API AOWNPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
	virtual void SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams()) override;

protected:
	void ApplySpringArmModifiers(float DeltaTime);
	void InitialiseDefaultsFromSpringArm(const USpringArmComponent* Component);

	UPROPERTY()
	FSpringArmProperties SpringArmDefaults;

	TWeakObjectPtr<USpringArmComponent> SpringArmComponent;
};
