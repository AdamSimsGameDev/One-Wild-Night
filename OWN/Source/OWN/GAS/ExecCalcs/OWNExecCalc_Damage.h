// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "OWNExecCalc_Damage.generated.h"

UCLASS()
class OWN_API UOWNExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
