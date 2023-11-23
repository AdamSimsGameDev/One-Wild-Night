// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "OWN/GameFramework/OWNPlayerState.h"
#include "ScoreManagerComponent.generated.h"

/**
 * 
 */
UCLASS()
class OWN_API UScoreManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
	
public:
	UScoreManagerComponent(const FObjectInitializer& objectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetWinnningScore(int32 score);

	const int32 GetWinningScore() {return WinningScore;}

	int32 GetPlayerScore(AOWNPlayerState* player);

	void SetPlayerScore(AOWNPlayerState* player, int32 score);
	
protected:
	UFUNCTION()
	void OnRep_WinningScore(int32 WinningScore);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = OnHandleWinningScoreReached))
	void K2_OnHandleWinningScoreReached();

	UFUNCTION(BlueprintCallable)
	void HandleWinningScoreReached();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, ReplicatedUsing=OnRep_WinningScore)
	int32 WinningScore = -1;
};
