// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors


#include "OWN/GameFramework/Component/ScoreManagerComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameplayMessageSubsystem.h"

UScoreManagerComponent::UScoreManagerComponent(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}

void UScoreManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UScoreManagerComponent, WinningScore);
}

void UScoreManagerComponent::SetWinnningScore(int32 score)
{
	WinningScore = score;
}

int32 UScoreManagerComponent::GetPlayerScore(AOWNPlayerState* player)
{
	if(player)
	{
		return player->GetStatTagContainer().GetStackCount(FGameplayTag()/* add score tag here*/);
	}
	else
	{
		return 0;
	}
}

void UScoreManagerComponent::OnRep_WinningScore(int32 targetScore)
{
	//broadcast winning score
	//UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	//MessageSystem.BroadcastMessage(TAG_Event_Game_WinningScore, message);
}

void UScoreManagerComponent::HandleWinningScoreReached()
{
	//broadcast winning tag
	//UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	//MessageSystem.BroadcastMessage(TAG_Event_Round_WinningScore_Reached, message);
}
