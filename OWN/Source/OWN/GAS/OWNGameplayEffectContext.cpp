// Fill out your copyright notice in the Description page of Project Settings.

#include "OWNGameplayEffectContext.h"

FOWNGameplayEffectContext* FOWNGameplayEffectContext::GetEffectContext(struct FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FOWNGameplayEffectContext::StaticStruct()))
	{
		return static_cast<FOWNGameplayEffectContext*>(BaseEffectContext);
	}

	return nullptr;
}