// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#include "WeaponGameplayAbility.h"
#include "OWN/GameFramework/WeaponDefinition.h"
#include "OWN/GameFramework/WeaponInstance.h"

AWeaponInstance* UWeaponGameplayAbility::GetWeaponInstance() const
{
	return Cast<AWeaponInstance>(GetCurrentSourceObject());
}

UWeaponDefinition* UWeaponGameplayAbility::GetWeaponDefinition() const
{
	if (AWeaponInstance* inst = GetWeaponInstance())
	{ 
		return inst->GetWeaponDefinition();
	}
	return nullptr;
}
