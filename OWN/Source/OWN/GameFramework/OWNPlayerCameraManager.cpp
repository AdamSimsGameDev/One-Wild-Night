// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#include "OWNPlayerCameraManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "OWN/GameFramework/OWNCameraModifier.h"
#include "OWN/OWNUtility.h"

void AOWNPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (SpringArmComponent.IsValid())
	{
		ApplySpringArmModifiers(DeltaTime);
	}
}

void AOWNPlayerCameraManager::SetViewTarget(AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams)
{
	Super::SetViewTarget(NewViewTarget, TransitionParams);

	SpringArmComponent = NewViewTarget->FindComponentByClass<USpringArmComponent>();
	InitialiseDefaultsFromSpringArm(GetDefaultComponent<USpringArmComponent>(NewViewTarget));
}

void AOWNPlayerCameraManager::ApplySpringArmModifiers(float DeltaTime)
{
	FSpringArmProperties Current = SpringArmDefaults;
	
	for (UCameraModifier* Modifier : ModifierList)
	{
		UOWNCameraModifier* MyModifier = Cast<UOWNCameraModifier>(Modifier);
		if ((MyModifier != nullptr) && !MyModifier->IsDisabled())
		{
			// If ModifySpringArm returns true, exit loop
			// Allows high priority things to dictate if they are
			// the last modifier to be applied
			if (MyModifier->ModifySpringArm(DeltaTime, Current))
			{
				break;
			}
		}
	}

	SpringArmComponent->TargetArmLength = Current.TargetArmLength;
	SpringArmComponent->TargetOffset = Current.TargetOffset;
	SpringArmComponent->SocketOffset = Current.SocketOffset;
	SpringArmComponent->SetRelativeLocation(Current.RelativeLocation);
	SpringArmComponent->SetRelativeRotation(Current.RelativeRotation);
}

void AOWNPlayerCameraManager::InitialiseDefaultsFromSpringArm(const USpringArmComponent* Component)
{
	if (!IsValid(Component))
	{
		return;
	}

	SpringArmDefaults.TargetArmLength = Component->TargetArmLength;
	SpringArmDefaults.TargetOffset = Component->TargetOffset;
	SpringArmDefaults.SocketOffset = Component->SocketOffset;
	SpringArmDefaults.RelativeLocation = Component->GetRelativeLocation();
	SpringArmDefaults.RelativeRotation = Component->GetRelativeRotation();
}
