// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterDefinition.generated.h"

class USkeletalMesh;
class UAbilitySet;
class UAnimInstance;

/**
 * 
 */
UCLASS(BlueprintType)
class OWN_API UCharacterDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Info)
	FText DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Info)
	USkeletalMesh* CharacterMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Info)
	FPrimaryAssetId WeaponDefinition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Info)
	TSubclassOf<UAnimInstance> AnimInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Info)
	UAbilitySet* AbilitySet;
};
