// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OWN/GAS/AbilitySet.h"
#include "OWN/Tags/GameplayStackTag.h"
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

	const FGameplayTagStackContainer& GetTagStackContainer() const { return TagStackContainer; }
	FGameplayTagStackContainer& GetTagStackContainer_Mutable() { return TagStackContainer; }

	void AddSpread();
private:
	void ComputeSpreadRange(float& MinSpread, float& MaxSpread);
	void ComputeHeatRange(float& MinHeat, float& MaxHeat);

	inline float ClampHeat(float NewHeat)
	{
		float MinHeat;
		float MaxHeat;
		ComputeHeatRange(/*out*/ MinHeat, /*out*/ MaxHeat);

		return FMath::Clamp(NewHeat, MinHeat, MaxHeat);
	}

	// Updates the spread and returns true if the spread is at minimum
	bool UpdateSpread(float DeltaSeconds);

	// Updates the multipliers and returns true if they are at minimum
	bool UpdateMultipliers(float DeltaSeconds);

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

	UPROPERTY(BlueprintReadWrite, Category = Weapon)
	FGameplayTagStackContainer TagStackContainer;

	UPROPERTY()
	FAbilitySetHandles AbilitySetHandles;

private:
	// Time since this weapon was last fired (relative to world time)
	double LastFireTime = 0.0;

	// The current heat
	float CurrentHeat = 0.0f;

	// The current spread angle (in degrees, diametrical)
	float CurrentSpreadAngle = 0.0f;

	// Do we currently have first shot accuracy?
	bool bHasFirstShotAccuracy = false;

	// The current *combined* spread angle multiplier
	float CurrentSpreadAngleMultiplier = 1.0f;

	// The current standing still multiplier
	float StandingStillMultiplier = 1.0f;

	// The current jumping/falling multiplier
	float JumpFallMultiplier = 1.0f;

	// The current crouching multiplier
	float CrouchingMultiplier = 1.0f;
};
