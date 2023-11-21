// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "OWN/GameFramework/OWNGameState.h"
#include "RoundStateManager.generated.h"


UENUM(BlueprintType)
enum class ERoundState : uint8
{
	// This value is essence exists as a default value for the Round State. Could help with potential race conditions at the beginning of games though
	init								UMETA(DisplayName = "itit"),
	WaitingForPlayers					UMETA(DisplayName = "Waiting For Players"),
	PreGame								UMETA(DisplayName = "Pre-Game"),
	Countdown							UMETA(DisplayName = "Pre-Round Countdown"),
	InProgress							UMETA(DisplayName = "Round In Progress"),
	RoundEnd							UMETA(DisplayName = "Round End"),

	Max	UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class OWN_API URoundStateManager : public UGameStateComponent
{
	GENERATED_BODY()
	
public:
	URoundStateManager(const FObjectInitializer& objectInitializer);

	virtual void InitializeComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPregame();
	virtual void BeginCountdown();
	virtual void BeginRound();
	virtual void OnRoundEnd();

	void SetRoundState(ERoundState newState);

	UFUNCTION(BlueprintPure, Category = "Round|Getters")
	ERoundState GetRoundState() const {return RoundState;}

	//UFUNCTION(BlueprintPure, Category = "Round|Getters")
	//int32 GetTimeLimit() const;

	//UFUNCTION(BlueprintPure, Category = "Round|Getters")
	//int32 GetTimeRemainingInRound() const { return GetTimeLimit() - ElapsedRoundTime; }

	//UFUNCTION(BlueprintPure, Category = "Round|Getters")
	//int32 GetTimeRemainingInPhase() const;

	UFUNCTION(BlueprintPure, Category = "Round|Getters")
	bool IsInRoundState(ERoundState roundState) const { return (uint8)RoundState == (uint8)roundState; }

	// Blueprint Functions
	UFUNCTION(BlueprintImplementableEvent, Category = "Round", meta = (DisplayName = "OnBeginPregame"))
	void K2_OnBeginPregame();
	UFUNCTION(BlueprintImplementableEvent, Category = "Round", meta = (DisplayName = "OnBeginCountdown"))
	void K2_OnBeginCountdown();
	UFUNCTION(BlueprintImplementableEvent, Category = "Round", meta = (DisplayName = "OnBeginRound"))
	void K2_OnBeginRound();
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "Round", meta = (DisplayName = "OnEndRound"))
	void K2_OnEndRound();
	UFUNCTION(BlueprintImplementableEvent, Category = "Round", meta = (DisplayName = "OnRoundStateChanged"))
	void K2_OnRoundStateChanged(ERoundState roundState);

	UFUNCTION()
	void OnRep_RoundPregameTime();

	UFUNCTION()
	void OnRep_RoundCountdownTime();

	UFUNCTION()
	void OnRep_RoundState(ERoundState oldState);

	UFUNCTION()
	void OnRep_ElapsedTime();

	UFUNCTION()
	void OnRep_RoundStartTime();

public:
	UPROPERTY(ReplicatedUsing = OnRep_RoundPregameTime)
	int32 RoundPregameTime;

	UPROPERTY(ReplicatedUsing = OnRep_RoundCountdownTime)
	int32 RoundCountdownTime;

	UPROPERTY(ReplicatedUsing = OnRep_ElapsedTime)
	int32 ElapsedTime;

	UPROPERTY(ReplicatedUsing = OnRep_RoundStartTime)
	float RoundStartTime;

	UPROPERTY(BlueprintReadOnly, Replicated)
	float StateChangeStartTime;

private:

	UPROPERTY(ReplicatedUsing = OnRep_RoundState)
	ERoundState RoundState;

	UPROPERTY()
	FTimerHandle RoundBeginTimer;

	UPROPERTY()
	FTimerHandle RoundTimer;

	AOWNGameState* GameState;
};
