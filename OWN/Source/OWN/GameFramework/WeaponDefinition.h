// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Info)
	int32 MagSize;

};
