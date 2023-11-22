// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OWN/Tags/GameplayStackTag.h"
#include "OWNBlueprintFunctionLibrary.generated.h"

class UTargetingSubsystem;
class UTargetingTask;
class UWeaponDefinition;

/**
 * 
 */
UCLASS()
class OWN_API UOWNBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Targeting", meta = (DefaultToSelf="Task"))
	static UTargetingSubsystem* GetTargetingSubsystemFromRequest(const UTargetingTask* Task, const FTargetingRequestHandle& TargetingHandle);

	UFUNCTION(BlueprintCallable, Category = "Stat Tags")
	static void AddStatTagStack(UPARAM(Ref)FGameplayTagStackContainer& Stack, FGameplayTag Tag, int32 Count);

	UFUNCTION(BlueprintCallable, Category = "Stat Tags")
	static void RemoveStatTagStack(UPARAM(Ref)FGameplayTagStackContainer& Stack, FGameplayTag Tag, int32 Count);

	UFUNCTION(BlueprintCallable, Category = "Stat Tags")
	static void SetStatTagStackCount(UPARAM(Ref)FGameplayTagStackContainer& Stack, FGameplayTag Tag, int32 Count);

	UFUNCTION(BlueprintPure, Category = "Stat Tags")
	static bool HasStatTagStack(const FGameplayTagStackContainer& Stack, FGameplayTag Tag);

	UFUNCTION(BlueprintPure, Category = "Stat Tags")
	static int32 GetStatTagStackCount(const FGameplayTagStackContainer& Stack, FGameplayTag Tag);

	UFUNCTION(BlueprintPure, Category = Weapon, meta = (DisplayName = "Get Weapon Data (Int)"))
	static int32 GetWeaponData_Int(const UWeaponDefinition* Definition, FGameplayTag Key);

	UFUNCTION(BlueprintPure, Category = Weapon, meta = (DisplayName = "Get Weapon Data (Bool)"))
	static bool GetWeaponData_Bool(const UWeaponDefinition* Definition, FGameplayTag Key);

	UFUNCTION(BlueprintPure, Category = Weapon, meta = (DisplayName = "Get Weapon Data (Float)"))
	static float GetWeaponData_Float(const UWeaponDefinition* Definition, FGameplayTag Key);

	UFUNCTION(BlueprintPure, Category = Weapon, meta = (DisplayName = "Get Weapon Data (Tag)"))
	static FGameplayTag GetWeaponData_Tag(const UWeaponDefinition* Definition, FGameplayTag Key);
};
