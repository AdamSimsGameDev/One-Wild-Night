// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "OWN/GameFramework/OWNPlayerCameraManager.h"
#include "OWNCameraModifier.generated.h"

/**
 * 
 */
UCLASS()
class OWN_API UOWNCameraModifier : public UCameraModifier
{
	GENERATED_BODY()
	
public:
	virtual bool ModifySpringArm(float DeltaTime, struct FSpringArmProperties& Props);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
	void BlueprintModifySpringArm(float DeltaTime, 
		float TargetArmLength, FVector SocketOffset, FVector TargetOffset, FVector RelativeLocation, FRotator RelativeRotation,
		float& NewTargetArmLength, FVector& NewSocketOffset, FVector& NewTargetOffset, FVector& NewRelativeLocation, FRotator& NewRelativeRotation);

};
