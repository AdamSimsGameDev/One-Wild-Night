// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "OWN/GAS/OWNGameplayAbility.h"
#include "WeaponGameplayAbility.generated.h"

class AWeaponInstance;
class UWeaponDefinition;

/**
 * 
 */
UCLASS()
class OWN_API UWeaponGameplayAbility : public UOWNGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	AWeaponInstance* GetWeaponInstance() const;

	UFUNCTION(BlueprintCallable)
	UWeaponDefinition* GetWeaponDefinition() const;
};
