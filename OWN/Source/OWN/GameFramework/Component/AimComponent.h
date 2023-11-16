// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AimComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OWN_API UAimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAimComponent();

protected:	
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Unreliable)
	void ServerSetCameraProps(FVector_NetQuantize100 location, FVector_NetQuantizeNormal direction);

protected:
	UPROPERTY()
	APawn* OwningPawn;

	UPROPERTY()
	class UCameraComponent* OwningCamera;

	UPROPERTY(BlueprintReadOnly, Replicated)
	FVector_NetQuantize100 AimLocation;

	UPROPERTY(BlueprintReadOnly, Replicated)
	FVector_NetQuantizeNormal AimDirection;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Aim)
	float AimDistance = 10000.0f;

	UPROPERTY(BlueprintReadOnly, Category = Aim)
	FHitResult LastHitLocation;
};
