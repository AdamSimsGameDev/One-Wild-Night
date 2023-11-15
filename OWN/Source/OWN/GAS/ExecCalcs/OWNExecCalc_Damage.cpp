// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#include "OWNExecCalc_Damage.h"

#include "OWN/GameFramework/Attribute/OWNHealthSet.h"
#include "OWN/GAS/OWNGameplayEffectContext.h"
#include "OWN/Tags/OWNTags.h"

void UOWNExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Get the effect spec
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Get tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Setup evaluation params
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	// Calculate damage
	float OutgoingDamage = 0.f;
	ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(TAG_Gameplay_Damage, EvaluateParameters, OutgoingDamage);

	// Apply damage to health set and mark conditional effects to trigger if damage has been dealt
	if (OutgoingDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UOWNHealthSet::GetDamageAttribute(), EGameplayModOp::Additive, OutgoingDamage));
		OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
	}
}
