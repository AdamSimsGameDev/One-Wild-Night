// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "OWNVerbMessage.h"
//#include "Engine/NetSerialization.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "OWNVerbMessageReplication.generated.h"

class UObject;
struct FOWNVerbMessageReplication;
struct FNetDeltaSerializeInfo;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FOWNVerbMessageReplicationEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FOWNVerbMessageReplicationEntry()
	{}

	FOWNVerbMessageReplicationEntry(const FOWNVerbMessage& InMessage)
		: Message(InMessage)
	{
	}

	FString GetDebugString() const;

private:
	friend FOWNVerbMessageReplication;

	UPROPERTY()
	FOWNVerbMessage Message;
};

/** Container of verb messages to replicate */
USTRUCT(BlueprintType)
struct FOWNVerbMessageReplication : public FFastArraySerializer
{
	GENERATED_BODY()

	FOWNVerbMessageReplication()
	{
	}

public:
	void SetOwner(UObject* InOwner) { Owner = InOwner; }

	// Broadcasts a message from server to clients
	void AddMessage(const FOWNVerbMessage& Message);

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FOWNVerbMessageReplicationEntry, FOWNVerbMessageReplication>(CurrentMessages, DeltaParms, *this);
	}

private:
	void RebroadcastMessage(const FOWNVerbMessage& Message);

private:
	// Replicated list of gameplay tag stacks
	UPROPERTY()
	TArray<FOWNVerbMessageReplicationEntry> CurrentMessages;
	
	// Owner (for a route to a world)
	UPROPERTY()
	TObjectPtr<UObject> Owner = nullptr;
};

template<>
struct TStructOpsTypeTraits<FOWNVerbMessageReplication> : public TStructOpsTypeTraitsBase2<FOWNVerbMessageReplication>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};