// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "MessageTypes.generated.h"

enum class ERoundState : uint8;

USTRUCT(BlueprintType)
struct OWN_API FRoundStateMessage
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	ERoundState RoundState;
};