// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OWN/GAS/OWNAbilitySystemComponent.h"
#include "OWNHealthComponent.generated.h"

class UOWNHealthComponent;

class UOWNAbilitySystemComponent;
class UOWNHealthSet;
class UObject;
struct FFrame;
struct FGameplayEffectSpec;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOWNHealth_DeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOWNHealth_AttributeChanged, UOWNHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);

/**
 * EOWNDeathState
 *
 *	Defines current state of death.
 */
UENUM(BlueprintType)
enum class EOWNDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OWN_API UOWNHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOWNHealthComponent(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	/** Gets the game instance this component is a part of, this will return null if not called during normal gameplay */
	template <class T>
	T* GetGameInstance() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, UGameInstance>::Value, "'T' template parameter to GetGameInstance must be derived from UGameInstance");
		AActor* Owner = GetOwner();
		return Owner ? Owner->GetGameInstance<T>() : nullptr;
	}

	template <class T>
	T* GetGameInstanceChecked() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, UGameInstance>::Value, "'T' template parameter to GetGameInstance must be derived from UGameInstance");
		AActor* Owner = GetOwner();
		check(Owner);
		T* GameInstance = Owner->GetGameInstance<T>();
		check(GameInstance);
		return GameInstance;
	}

	/** Returns true if the owner's role is ROLE_Authority */
	bool HasAuthority() const;

	/** Returns the world's timer manager */
	class FTimerManager& GetWorldTimerManager() const;

	// Returns the health component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "OWN|Health")
	static UOWNHealthComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UOWNHealthComponent>() : nullptr); }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "OWN|Health")
	void InitializeWithAbilitySystem(UOWNAbilitySystemComponent* InASC);

	// Uninitialize the component, clearing any references to the ability system.
	UFUNCTION(BlueprintCallable, Category = "OWN|Health")
	void UninitializeFromAbilitySystem();

	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "OWN|Health")
	float GetHealth() const;

	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "OWN|Health")
	float GetMaxHealth() const;

	// Returns the current health in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "OWN|Health")
	float GetHealthNormalized() const;

	UFUNCTION(BlueprintCallable, Category = "OWN|Health")
	EOWNDeathState GetDeathState() const { return DeathState; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "OWN|Health", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDeadOrDying() const { return (DeathState > EOWNDeathState::NotDead); }

	// Begins the death sequence for the owner.
	virtual void StartDeath();

	// Ends the death sequence for the owner.
	virtual void FinishDeath();

	// Applies enough damage to kill the owner.
	virtual void DamageSelfDestruct(bool bFellOutOfWorld = false);

public:

	// Delegate fired when the health value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FOWNHealth_AttributeChanged OnHealthChanged;

	// Delegate fired when the max health value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FOWNHealth_AttributeChanged OnMaxHealthChanged;

	// Delegate fired when the death sequence has started.
	UPROPERTY(BlueprintAssignable)
	FOWNHealth_DeathEvent OnDeathStarted;

	// Delegate fired when the death sequence has finished.
	UPROPERTY(BlueprintAssignable)
	FOWNHealth_DeathEvent OnDeathFinished;

protected:

	virtual void OnUnregister() override;

	void ClearGameplayTags();

	virtual void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	virtual void HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

	UFUNCTION()
	virtual void OnRep_DeathState(EOWNDeathState OldDeathState);

protected:

	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UOWNAbilitySystemComponent> AbilitySystemComponent;

	// Health set used by this component.
	UPROPERTY()
	TObjectPtr<const UOWNHealthSet> HealthSet;

	// Replicated state used to handle dying.
	UPROPERTY(ReplicatedUsing = OnRep_DeathState)
	EOWNDeathState DeathState;
};

/**
 * Const iterator for registered components on an actor
 */
template <typename T>
class TConstComponentIterator
{
public:
	explicit TConstComponentIterator(const AActor* OwnerActor)
		: CompIndex(-1)
	{
		if (IsValid(OwnerActor))
		{
			OwnerActor->GetComponents(AllComponents);
		}

		Advance();
	}

	FORCEINLINE void operator++()
	{
		Advance();
	}

	FORCEINLINE explicit operator bool() const
	{
		return AllComponents.IsValidIndex(CompIndex);
	}

	FORCEINLINE bool operator!() const
	{
		return !(bool)*this;
	}

	FORCEINLINE const T* operator*() const
	{
		return GetComponent();
	}

	FORCEINLINE const T* operator->() const
	{
		return GetComponent();
	}

protected:
	/** Gets the current component */
	FORCEINLINE const T* GetComponent() const
	{
		return AllComponents[CompIndex];
	}

	/** Moves the iterator to the next valid component */
	FORCEINLINE bool Advance()
	{
		while (++CompIndex < AllComponents.Num())
		{
			const T* Comp = GetComponent();
			check(Comp);
			if (Comp->IsRegistered())
			{
				checkf(IsValidChecked(Comp), TEXT("Registered game framework component was invalid! Comp: %s"), *GetPathNameSafe(Comp));
				return true;
			}
		}

		return false;
	}

private:
	/** Results from GetComponents */
	TInlineComponentArray<const T*> AllComponents;

	/** Index of the current element in the componnet array */
	int32 CompIndex;

	FORCEINLINE bool operator==(const TConstComponentIterator& Other) const { return CompIndex == Other.CompIndex; }
	FORCEINLINE bool operator!=(const TConstComponentIterator& Other) const { return CompIndex != Other.CompIndex; }
};
