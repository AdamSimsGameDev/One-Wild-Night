// Copyright Epic Games, Inc. All Rights Reserved.

#include "OWNCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerState.h"
#include "OWN/GameFramework/CharacterDefinition.h"
#include "OWN/GameFramework/WeaponDefinition.h"
#include "OWN/GameFramework/WeaponInstance.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "OWN/GAS/OWNAbilitySystemComponent.h"
#include "Component/AimComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/AssetManager.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AOWNCharacter

AOWNCharacter::AOWNCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AbilitySystemComponent = CreateDefaultSubobject<UOWNAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	HealthComponent = CreateDefaultSubobject<UOWNHealthComponent>(TEXT("HealthComp"));
	AimComponent = CreateDefaultSubobject<UAimComponent>(TEXT("AimComponent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

UAbilitySystemComponent* AOWNCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AOWNCharacter::SetWeapon(FPrimaryAssetId assetId)
{
	CurrentWeaponId = assetId;
	UAssetManager& assetManager = UAssetManager::Get();
	assetManager.LoadPrimaryAsset(assetId, { }, FStreamableDelegate::CreateUObject(this, &AOWNCharacter::OnWeaponDefinitionLoaded, assetId));
}

void AOWNCharacter::OnWeaponDefinitionLoaded(FPrimaryAssetId assetId)
{
	if (assetId != CurrentWeaponId)
		return;
	UAssetManager& assetManager = UAssetManager::Get();
	SetWeapon(CastChecked<UWeaponDefinition>(assetManager.GetPrimaryAssetObject(assetId)));
}

void AOWNCharacter::SetWeapon(UWeaponDefinition* definition)
{
	if (!HasAuthority())
		return;

	if (CurrentWeapon)
	{
		CurrentWeapon->OnRemove();
		CurrentWeapon->Destroy();
	}

	CurrentWeapon = nullptr;

	if (definition)
	{
		// spawn the weapon actor
		AWeaponInstance* instance = GetWorld()->SpawnActorDeferred<AWeaponInstance>(definition->WeaponInstanceClass.Get(), FTransform(), this, this);
		// say that we prespawned the actor
		instance->OnPreSpawned();
		// set the weapon actor
		instance->FinishSpawning(FTransform());
		// attach the weapon actor
		instance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r"));
		// tell the weapon it has been spawned
		instance->OnSpawned();

		CurrentWeapon = instance;
	}

	OnRep_CurrentWeapon();
}

void AOWNCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOWNCharacter, ControlRotationDirection);
	DOREPLIFETIME(AOWNCharacter, CharacterDefinition);
	DOREPLIFETIME(AOWNCharacter, CurrentWeapon);
}

void AOWNCharacter::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);

	AbilitySystemComponent->InitAbilityActorInfo(GetPlayerState(), this);

	APlayerController* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		BindInputToController(pc);
	}

	LastController = pc;
}

void AOWNCharacter::UnPossessed()
{
	Super::UnPossessed();

	AbilitySystemComponent->ClearActorInfo();

	if (LastController.IsValid())
	{
		UnbindInputFromController(LastController.Get());
	}
}

void AOWNCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(GetPlayerState(), this);
}

void AOWNCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	APlayerController* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		BindInputToController(pc);
	}
	else if (LastController.IsValid())
	{
		UnbindInputFromController(LastController.Get());
	}

	LastController = pc;
}

void AOWNCharacter::Move(FVector2D Value)
{
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Value.Y);
		AddMovementInput(RightDirection, Value.X);

		K2_OnMove(Value);
	}
}

void AOWNCharacter::Look(FVector2D Value)
{
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Value.X);
		AddControllerPitchInput(Value.Y);
	}
}

void AOWNCharacter::BindInputToController(APlayerController* controller)
{
	if (!IsValid(controller) || !controller->IsLocalController())
	{
		return;
	}

	FModifyContextOptions options;
	options.bForceImmediately = false;
	options.bIgnoreAllPressedKeysUntilRelease = true;

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer());
	if (subsystem)
	{
		subsystem->AddMappingContext(InputMappingContext, InputMappingPriority, options);
	}
}

void AOWNCharacter::UnbindInputFromController(APlayerController* controller)
{
	if (!IsValid(controller) || !controller->IsLocalController())
	{
		return;
	}

	FModifyContextOptions options;
	options.bForceImmediately = false;
	options.bIgnoreAllPressedKeysUntilRelease = true;

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer());
	if (subsystem)
	{
		subsystem->RemoveMappingContext(InputMappingContext, options);
	}
}

void AOWNCharacter::OnRep_CharacterDefinition()
{
	UE_LOG(LogTemplateCharacter, Log, TEXT("Character Definition for pawn %s set to %s, name is %s. "), *GetName(), *GetNameSafe(CharacterDefinition), CharacterDefinition ? *CharacterDefinition->DisplayName.ToString() : TEXT("NONE"));

	GetMesh()->SetSkeletalMesh(CharacterDefinition->CharacterMesh);
	GetMesh()->SetAnimInstanceClass(CharacterDefinition->AnimInstance.Get());

	K2_OnCharacterDefinitionSet(CharacterDefinition);

	OnCharacterDefinitionSet.Broadcast(this, CharacterDefinition);
}

void AOWNCharacter::OnRep_CurrentWeapon()
{
	K2_OnCurrentWeaponSet(CurrentWeapon);
}

void AOWNCharacter::SetCharacterDefinition(UCharacterDefinition* definition)
{
	if (!IsNetMode(NM_Client))
	{
		if (CharacterDefinition)
		{
			UAbilitySet::RemoveAbilitySet(AbilitySystemComponent, AbilitySetHandles);
			SetWeapon(nullptr);
		}

		CharacterDefinition = definition;
		OnRep_CharacterDefinition();

		if (CharacterDefinition)
		{
			AbilitySetHandles = UAbilitySet::AddAbilitySet(definition->AbilitySet, AbilitySystemComponent);
			SetWeapon(definition->WeaponDefinition);
		}
	}
	else if (IsLocallyControlled())
	{
		ServerSetCharacterDefinition(definition);
	}
}

void AOWNCharacter::ServerSetCharacterDefinition_Implementation(UCharacterDefinition* definition)
{
	SetCharacterDefinition(definition);
}
