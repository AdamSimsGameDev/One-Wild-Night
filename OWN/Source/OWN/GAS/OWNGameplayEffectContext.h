// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "OWNGameplayEffectContext.generated.h"

class UAbilitySystemComponent;

USTRUCT()
struct FOWNGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	/** Returns the wrapped FOWNGameplayEffectContext from the handle, or nullptr if it doesn't exist or is the wrong type */
	static OWN_API FOWNGameplayEffectContext* GetEffectContext(struct FGameplayEffectContextHandle Handle);

	virtual FGameplayEffectContext* Duplicate() const override
	{
		FOWNGameplayEffectContext* NewContext = new FOWNGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FOWNGameplayEffectContext::StaticStruct();
	}
};

template<>
struct TStructOpsTypeTraits<FOWNGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FOWNGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
