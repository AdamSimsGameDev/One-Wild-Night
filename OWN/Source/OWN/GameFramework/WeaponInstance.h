// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OWN/GAS/AbilitySet.h"
#include "WeaponInstance.generated.h"

class UWeaponDefinition;

UCLASS()
class OWN_API AWeaponInstance : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponInstance();

	UWeaponDefinition* GetWeaponDefinition() const { return WeaponDefinition; }
	
	virtual void OnPreSpawned();
	virtual void OnSpawned();
	virtual void OnRemove();

	UFUNCTION(BlueprintCallable)
	virtual FVector GetMuzzleLocation() const;

protected:
	void GrantAbilitiesToOwner();
	void RemoveAbilitiesFromOwner();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Get Muzzle Location"))
	FVector K2_GetMuzzleLocation() const;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Pre Spawned"))
	void K2_OnPreSpawned();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Spawned"))
	void K2_OnSpawned();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Remove"))
	void K2_OnRemove();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Weapon)
	UWeaponDefinition* WeaponDefinition;

	UPROPERTY()
	FAbilitySetHandles AbilitySetHandles;
};
