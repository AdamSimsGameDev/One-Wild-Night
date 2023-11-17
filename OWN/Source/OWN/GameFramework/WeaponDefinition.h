// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "WeaponDefinition.generated.h"

class USkeletalMesh;
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
	USkeletalMesh* WeaponMesh;

	template<typename PropertyType>
	PropertyType GetStat(FGameplayTag tag);

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, int32> IntMap;
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, bool> BoolMap;
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, float> FloatMap;

};

//template Specializations
template<>
inline int32 UWeaponDefinition::GetStat<int32>(FGameplayTag tag)
{
	return IntMap.FindChecked(tag);
}

template<>
inline bool UWeaponDefinition::GetStat<bool>(FGameplayTag tag)
{
	return BoolMap.FindChecked(tag);
}

template<>
inline float UWeaponDefinition::GetStat<float>(FGameplayTag tag)
{
	return FloatMap.FindChecked(tag);
}
