// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#include "WeaponInstance.h"
#include "OWN/OWNUtility.h"
#include "OWN/GameFramework/WeaponDefinition.h"
#include "OWN/GAS/OWNAbilitySystemGlobals.h"

AWeaponInstance::AWeaponInstance()
{
	bReplicates = true;
}

void AWeaponInstance::OnPreSpawned()
{
	GrantAbilitiesToOwner();

	K2_OnPreSpawned();
}

void AWeaponInstance::OnSpawned()
{
	K2_OnSpawned();
}

void AWeaponInstance::OnRemove()
{
	RemoveAbilitiesFromOwner();

	K2_OnRemove();
}

FVector AWeaponInstance::GetMuzzleLocation() const
{
	if (IsFunctionImplementedInBlueprint(K2_GetMuzzleLocation))
	{
		return K2_GetMuzzleLocation();
	}
	return FVector::ZeroVector;
}

void AWeaponInstance::GrantAbilitiesToOwner()
{
	AbilitySetHandles = UAbilitySet::AddAbilitySet(WeaponDefinition->AbilitySet, UOWNAbilitySystemGlobals::GetOWNAbilitySystemComponentFromActor(GetOwner()));
}

void AWeaponInstance::RemoveAbilitiesFromOwner()
{
	UAbilitySet::RemoveAbilitySet(UOWNAbilitySystemGlobals::GetOWNAbilitySystemComponentFromActor(GetOwner()), AbilitySetHandles);
}
