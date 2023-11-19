// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors


#include "OWN/GAS/Targeting/OWNGATargetData_SingleTargetData.h"
//#include "GameplayEffectContext.h"


struct FGameplayEffectContextHandle;

//////////////////////////////////////////////////////////////////////

void FUOWNGATargetData_SingleTargetHit::AddTargetDataToContext(FGameplayEffectContextHandle& Context, bool bIncludeActorArray) const
{
	FGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(Context, bIncludeActorArray);

	// Add game-specific data
	//if (FGameplayEffectContext* TypedContext = FGameplayEffectContext::ExtractEffectContext(Context))
	//{
	//	TypedContext->CartridgeID = CartridgeID;
	//}
}

bool FUOWNGATargetData_SingleTargetHit::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayAbilityTargetData_SingleTargetHit::NetSerialize(Ar, Map, bOutSuccess);

	Ar << CartridgeID;

	return true;
}
