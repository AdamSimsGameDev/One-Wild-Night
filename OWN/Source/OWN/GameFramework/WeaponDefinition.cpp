// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#include "WeaponDefinition.h"

static const FName NAME_WeaponDefinition = TEXT("WeaponDefinition");


FPrimaryAssetId UWeaponDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(NAME_WeaponDefinition, GetFName());
}
