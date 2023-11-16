// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OWNBlueprintFunctionLibrary.generated.h"

class UTargetingSubsystem;
class UTargetingTask;

/**
 * 
 */
UCLASS()
class OWN_API UOWNBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Targeting", meta = (DefaultToSelf="Task"))
	static UTargetingSubsystem* GetTargetingSubsystemFromRequest(const UTargetingTask* Task, const FTargetingRequestHandle& TargetingHandle);
};
