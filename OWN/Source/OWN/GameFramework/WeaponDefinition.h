// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "WeaponDefinition.generated.h"

class USkeletalMesh;
class UAbilitySet;

/**
 * 
 */
UCLASS()
class OWN_API UWeaponDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Info)
	FText DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Info)
	TSubclassOf<class AWeaponInstance> WeaponInstanceClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Info)
	UAbilitySet* AbilitySet;

	template<typename PropertyType>
	PropertyType GetStat(FGameplayTag tag) const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, int32> IntMap;
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, bool> BoolMap;
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, float> FloatMap;
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FGameplayTag> TagMap;
};

//template Specializations
template<>
inline int32 UWeaponDefinition::GetStat<int32>(FGameplayTag tag) const
{
	return IntMap.FindChecked(tag);
}

template<>
inline bool UWeaponDefinition::GetStat<bool>(FGameplayTag tag) const
{
	return BoolMap.FindChecked(tag);
}

template<>
inline float UWeaponDefinition::GetStat<float>(FGameplayTag tag) const
{
	return FloatMap.FindChecked(tag);
}

template<>
inline FGameplayTag UWeaponDefinition::GetStat<FGameplayTag>(FGameplayTag tag) const
{
	return TagMap.FindChecked(tag);
}
