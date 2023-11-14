// Fill out your copyright notice in the Description page of Project Settings.


#include "OWNGameplayMessageProcess.h"

#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"

// Called when the game starts
void UOWNGameplayMessageProcess::BeginPlay()
{
	Super::BeginPlay();

	StartListening();
	
}

void UOWNGameplayMessageProcess::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	StopListening();

	// Remove any listener handles
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	for (FGameplayMessageListenerHandle& Handle : ListenerHandles)
	{
		MessageSubsystem.UnregisterListener(Handle);
	}
	ListenerHandles.Empty();
}

void UOWNGameplayMessageProcess::StartListening()
{
}

void UOWNGameplayMessageProcess::StopListening()
{
}

void UOWNGameplayMessageProcess::AddListenerHandle(FGameplayMessageListenerHandle&& Handle)
{
	ListenerHandles.Add(MoveTemp(Handle));
}

double UOWNGameplayMessageProcess::GetServerTime() const
{
	if (AGameStateBase* GameState = GetWorld()->GetGameState())
	{
		return GameState->GetServerWorldTimeSeconds();
	}
	else
	{
		return 0.0;
	}
}
