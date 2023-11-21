// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#include "AimComponent.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"

UAimComponent::UAimComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	SetIsReplicatedByDefault(true);
}

void UAimComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPawn = GetOwner<APawn>();
	OwningCamera = OwningPawn ? OwningPawn->FindComponentByClass<UCameraComponent>() : nullptr;
}

void UAimComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UAimComponent, AimLocation, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UAimComponent, AimDirection, COND_SkipOwner);
}

void UAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AController* controller = OwningPawn ? OwningPawn->GetController() : nullptr;
	bool isLocallyControlled = controller ? controller->IsLocalController() : false;

	if (isLocallyControlled)
	{
		AimLocation = OwningCamera->GetComponentLocation();
		AimDirection = OwningCamera->GetForwardVector();
		ServerSetCameraProps(AimLocation, AimDirection);
	}

	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(GetOwner());

	const FVector EndLocation = AimLocation + (AimDirection * AimDistance);

	const bool hit = GetWorld()->LineTraceSingleByChannel(
		LastHitLocation, 
		AimLocation,
		EndLocation,
		ECollisionChannel::ECC_WorldStatic,
		queryParams,
		FCollisionResponseParams::DefaultResponseParam
	);
	if (!hit)
	{
		LastHitLocation.Location = EndLocation;
		LastHitLocation.ImpactPoint = EndLocation;
	}
}

void UAimComponent::ServerSetCameraProps_Implementation(FVector_NetQuantize100 location, FVector_NetQuantizeNormal direction)
{
	AimLocation = location;
	AimDirection = direction;
}
