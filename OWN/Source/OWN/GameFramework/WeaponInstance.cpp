// Made by these people: https://github.com/AdamSimsGameDev/One-Wild-Night/graphs/contributors

#include "WeaponInstance.h"
#include "OWN/OWNUtility.h"
#include "OWN/GameFramework/WeaponDefinition.h"
#include "OWN/GAS/OWNAbilitySystemGlobals.h"

AWeaponInstance::AWeaponInstance()
{
	bReplicates = true;
}

void AWeaponInstance::OnPreSpawned()
{
	GrantAbilitiesToOwner();

	K2_OnPreSpawned();
}

void AWeaponInstance::OnSpawned()
{
	K2_OnSpawned();
}

void AWeaponInstance::OnRemove()
{
	RemoveAbilitiesFromOwner();

	K2_OnRemove();
}

FVector AWeaponInstance::GetMuzzleLocation() const
{
	if (IsFunctionImplementedInBlueprint(K2_GetMuzzleLocation))
	{
		return K2_GetMuzzleLocation();
	}
	return FVector::ZeroVector;
}

void AWeaponInstance::GrantAbilitiesToOwner()
{
	AbilitySetHandles = UAbilitySet::AddAbilitySet(WeaponDefinition->AbilitySet, UOWNAbilitySystemGlobals::GetOWNAbilitySystemComponentFromActor(GetOwner()));
}

void AWeaponInstance::RemoveAbilitiesFromOwner()
{
	UAbilitySet::RemoveAbilitySet(UOWNAbilitySystemGlobals::GetOWNAbilitySystemComponentFromActor(GetOwner()), AbilitySetHandles);
}

void AWeaponInstance::AddSpread()
{
	// Sample the heat up curve
	const float HeatPerShot = WeaponDefinition->HeatToHeatPerShotCurve.GetRichCurveConst()->Eval(CurrentHeat);
	CurrentHeat = ClampHeat(CurrentHeat + HeatPerShot);

	// Map the heat to the spread angle
	CurrentSpreadAngle = WeaponDefinition->HeatToSpreadCurve.GetRichCurveConst()->Eval(CurrentHeat);
}

void AWeaponInstance::ComputeSpreadRange(float& MinSpread, float& MaxSpread)
{
	WeaponDefinition->HeatToSpreadCurve.GetRichCurveConst()->GetValueRange(/*out*/ MinSpread, /*out*/ MaxSpread);
}

void AWeaponInstance::ComputeHeatRange(float& MinHeat, float& MaxHeat)
{
	float Min1;
	float Max1;
	WeaponDefinition->HeatToHeatPerShotCurve.GetRichCurveConst()->GetTimeRange(/*out*/ Min1, /*out*/ Max1);

	float Min2;
	float Max2;
	WeaponDefinition->HeatToCoolDownPerSecondCurve.GetRichCurveConst()->GetTimeRange(/*out*/ Min2, /*out*/ Max2);

	float Min3;
	float Max3;
	WeaponDefinition->HeatToSpreadCurve.GetRichCurveConst()->GetTimeRange(/*out*/ Min3, /*out*/ Max3);

	MinHeat = FMath::Min(FMath::Min(Min1, Min2), Min3);
	MaxHeat = FMath::Max(FMath::Max(Max1, Max2), Max3);
}

bool AWeaponInstance::UpdateSpread(float DeltaSeconds)
{
	const float TimeSinceFired = GetWorld()->TimeSince(LastFireTime);

	if (TimeSinceFired > WeaponDefinition->SpreadRecoveryCooldownDelay)
	{
		const float CooldownRate = WeaponDefinition->HeatToCoolDownPerSecondCurve.GetRichCurveConst()->Eval(CurrentHeat);
		CurrentHeat = ClampHeat(CurrentHeat - (CooldownRate * DeltaSeconds));
		CurrentSpreadAngle = WeaponDefinition->HeatToSpreadCurve.GetRichCurveConst()->Eval(CurrentHeat);
	}

	float MinSpread;
	float MaxSpread;
	ComputeSpreadRange(/*out*/ MinSpread, /*out*/ MaxSpread);

	return FMath::IsNearlyEqual(CurrentSpreadAngle, MinSpread, KINDA_SMALL_NUMBER);
}

bool AWeaponInstance::UpdateMultipliers(float DeltaSeconds)
{
	const float MultiplierNearlyEqualThreshold = 0.05f;

	//APawn* Pawn = GetPawn();
	//check(Pawn != nullptr);
	//UCharacterMovementComponent* CharMovementComp = Cast<UCharacterMovementComponent>(Pawn->GetMovementComponent());

	// See if we are standing still, and if so, smoothly apply the bonus
	//const float PawnSpeed = Pawn->GetVelocity().Size();
	const float MovementTargetValue = FMath::GetMappedRangeValueClamped(
		/*InputRange=*/ FVector2D(WeaponDefinition->StandingStillSpeedThreshold, WeaponDefinition->StandingStillSpeedThreshold + WeaponDefinition->StandingStillToMovingSpeedRange),
		/*OutputRange=*/ FVector2D(WeaponDefinition->SpreadAngleMultiplier_StandingStill, 1.0f),
		/*Alpha=*/ /*PawnSpeed*/ 0);
	StandingStillMultiplier = FMath::FInterpTo(StandingStillMultiplier, MovementTargetValue, DeltaSeconds, WeaponDefinition->TransitionRate_StandingStill);
	const bool bStandingStillMultiplierAtMin = FMath::IsNearlyEqual(StandingStillMultiplier, WeaponDefinition->SpreadAngleMultiplier_StandingStill, WeaponDefinition->SpreadAngleMultiplier_StandingStill * 0.1f);

	// See if we are crouching, and if so, smoothly apply the bonus
	const bool bIsCrouching = false;/*(CharMovementComp != nullptr) && CharMovementComp->IsCrouching();*/
	const float CrouchingTargetValue = bIsCrouching ? WeaponDefinition->SpreadAngleMultiplier_Crouching : 1.0f;
	CrouchingMultiplier = FMath::FInterpTo(CrouchingMultiplier, CrouchingTargetValue, DeltaSeconds, WeaponDefinition->TransitionRate_Crouching);
	const bool bCrouchingMultiplierAtTarget = FMath::IsNearlyEqual(CrouchingMultiplier, CrouchingTargetValue, MultiplierNearlyEqualThreshold);

	// See if we are in the air (jumping/falling), and if so, smoothly apply the penalty
	const bool bIsJumpingOrFalling = false/*(CharMovementComp != nullptr) && CharMovementComp->IsFalling()*/;
	const float JumpFallTargetValue = bIsJumpingOrFalling ? WeaponDefinition->SpreadAngleMultiplier_JumpingOrFalling : 1.0f;
	JumpFallMultiplier = FMath::FInterpTo(JumpFallMultiplier, JumpFallTargetValue, DeltaSeconds, WeaponDefinition->TransitionRate_JumpingOrFalling);
	const bool bJumpFallMultiplerIs1 = FMath::IsNearlyEqual(JumpFallMultiplier, 1.0f, MultiplierNearlyEqualThreshold);

	// Determine if we are aiming down sights, and apply the bonus based on how far into the camera transition we are
	float AimingAlpha = 0.0f;
	/*if (const ULyraCameraComponent* CameraComponent = ULyraCameraComponent::FindCameraComponent(Pawn))
	{
		float TopCameraWeight;
		FGameplayTag TopCameraTag;
		CameraComponent->GetBlendInfo(/*out*//* TopCameraWeight, /*out*/ /*TopCameraTag);

		AimingAlpha = (TopCameraTag == TAG_Lyra_Weapon_SteadyAimingCamera) ? TopCameraWeight : 0.0f;
	}*/
	const float AimingMultiplier = FMath::GetMappedRangeValueClamped(
		/*InputRange=*/ FVector2D(0.0f, 1.0f),
		/*OutputRange=*/ FVector2D(1.0f, WeaponDefinition->SpreadAngleMultiplier_Aiming),
		/*Alpha=*/ AimingAlpha);
	const bool bAimingMultiplierAtTarget = FMath::IsNearlyEqual(AimingMultiplier, WeaponDefinition->SpreadAngleMultiplier_Aiming, KINDA_SMALL_NUMBER);

	// Combine all the multipliers
	const float CombinedMultiplier = AimingMultiplier * StandingStillMultiplier * CrouchingMultiplier * JumpFallMultiplier;
	CurrentSpreadAngleMultiplier = CombinedMultiplier;

	// need to handle these spread multipliers indicating we are not at min spread
	return bStandingStillMultiplierAtMin && bCrouchingMultiplierAtTarget && bJumpFallMultiplerIs1 && bAimingMultiplierAtTarget;
}