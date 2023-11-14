// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterDefinition.h"

static const FName NAME_CharacterDefinition = TEXT("CharacterDefinition");

FPrimaryAssetId UCharacterDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(NAME_CharacterDefinition, GetFName());
}