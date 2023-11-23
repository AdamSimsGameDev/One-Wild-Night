// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors


#include "RoundStateManager.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "OWN/Messages/MessageTypes.h"
#include "OWN/Tags/OWNTags.h"

URoundStateManager::URoundStateManager(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	SetIsReplicatedByDefault(true);
	bAutoRegister = true;
	bAutoActivate = true;
	bWantsInitializeComponent = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}



void URoundStateManager::InitializeComponent()
{
	Super::InitializeComponent();
	GameState = GetGameStateChecked<AOWNGameState>();
}

void URoundStateManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URoundStateManager, RoundPregameTime);
	DOREPLIFETIME(URoundStateManager, RoundCountdownTime);
	DOREPLIFETIME(URoundStateManager, ElapsedTime);
	DOREPLIFETIME(URoundStateManager, RoundStartTime);
	DOREPLIFETIME(URoundStateManager, StateChangeStartTime);
	DOREPLIFETIME(URoundStateManager, RoundState);
}

void URoundStateManager::BeginPregame()
{
	K2_OnBeginPregame();
	SetRoundState(ERoundState::PreGame);

	//get the pregame set time from the game mode
	if (RoundPregameTime != 0)
	{
		GetWorld()->GetTimerManager().SetTimer(RoundBeginTimer, this, &URoundStateManager::BeginCountdown, RoundPregameTime);
	}
	else
	{
		BeginCountdown();
	}
}

void URoundStateManager::BeginCountdown()
{
	K2_OnBeginCountdown();
	SetRoundState(ERoundState::Countdown);

	//get the countdown time from the game mode
	if (RoundCountdownTime != 0)
	{
		GetWorld()->GetTimerManager().SetTimer(RoundBeginTimer, this, &URoundStateManager::BeginRound, RoundCountdownTime);
	}
	else
	{
		BeginRound();
	}
}

void URoundStateManager::BeginRound()
{
	K2_OnBeginRound();
	SetRoundState(ERoundState::InProgress);

	//get the time limit from the game mode
	//if (timeLimit > 0)
	//{
		//GetWorldTimerManager().SetTimer(RoundTimer, this, &URoundStateManager::OnRoundEnd, timeLimit, false);
	//}
}

void URoundStateManager::OnRoundEnd()
{
	K2_OnEndRound();
	SetRoundState(ERoundState::RoundEnd);
}

void URoundStateManager::SetRoundState(ERoundState newState)
{
	ensure(HasAuthority());

	if (URoundStateManager* roundStateManager = GameState->FindComponentByClass<URoundStateManager>())
	{
		//need to grab any round setting here

		ERoundState oldState = RoundState;
		//MARK_PROPERTY_DIRTY_FROM_NAME(URoundStateManager, RoundState, this);
		RoundState = newState;

		switch (RoundState)
		{
		case ERoundState::WaitingForPlayers:
			

			break;
		case ERoundState::PreGame:
			//set pregame time here

			break;
		case ERoundState::Countdown:
			//set coutdown duration here

			break;
		case ERoundState::InProgress:
			// Set Round start time

			break;
		case ERoundState::RoundEnd:
			//reset bit here if playing another

			break;
		case ERoundState::Max:
		default:
			break;
		}

		OnRep_RoundState(oldState);
	}

}

/*int32 URoundStateManager::GetTimeLimit() const
{
	//get the time limit from the game mode
}

int32 URoundStateManager::GetTimeRemainingInState() const
{
	//get the time remaining in a specific state
}*/

void URoundStateManager::OnRep_RoundPregameTime()
{
	//need to figure out the message we wanna send from this
	//UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	//MessageSystem.BroadcastMessage(TAG_Event_Round_Time_PregameTime, message);
}

void URoundStateManager::OnRep_RoundCountdownTime()
{
	//UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	//MessageSystem.BroadcastMessage(TAG_GameplayEvent_Round_CountdownTime, Message);
}

void URoundStateManager::OnRep_RoundState(ERoundState oldState)
{
	// These functions are handled in a more accurate way for the server
	if (!HasAuthority())
	{
		switch (RoundState)
		{
		case ERoundState::PreGame:
			K2_OnBeginPregame();
			break;
		case ERoundState::Countdown:
			K2_OnBeginCountdown();
			break;
		case ERoundState::InProgress:
			K2_OnBeginRound();
			break;
		default:
			break;
		}
	}

	// Let the blueprint know that the round state has changed
	K2_OnRoundStateChanged(RoundState);

	FRoundStateMessage message;
	message.RoundState = RoundState;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(TAG_GameplayEvent_Round_State, message);
}

void URoundStateManager::OnRep_ElapsedTime()
{
	//UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	//MessageSystem.BroadcastMessage(TAG_GameplayEvent_Round_ElapsedTime, Message);
}

void URoundStateManager::OnRep_RoundStartTime()
{
	//UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	//MessageSystem.BroadcastMessage(TAG_GameplayEvent_Round_StartTime, Message);
}
