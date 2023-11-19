// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "OWNGATargetData_SingleTargetData.generated.h"

/**
 * 
 */
USTRUCT()
struct FUOWNGATargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()

	FUOWNGATargetData_SingleTargetHit()
		: CartridgeID(-1)
	{ }

	virtual void AddTargetDataToContext(FGameplayEffectContextHandle& Context, bool bIncludeActorArray) const override;

	/** ID to allow the identification of multiple bullets that were part of the same cartridge */
	UPROPERTY()
	int32 CartridgeID;

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FUOWNGATargetData_SingleTargetHit::StaticStruct();
	}
};

template<>
struct TStructOpsTypeTraits<FUOWNGATargetData_SingleTargetHit> : public TStructOpsTypeTraitsBase2<FUOWNGATargetData_SingleTargetHit>
{
	enum
	{
		WithNetSerializer = true	// For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};

