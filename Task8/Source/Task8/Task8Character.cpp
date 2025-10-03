// Copyright Epic Games, Inc. All Rights Reserved.

#include "Task8Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "Task8/Task8PlayerController.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GAS/TaskAttributeSet.h"

ATask8Character::ATask8Character()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttrSet = CreateDefaultSubobject<UTaskAttributeSet>(TEXT("AttrSet"));
}

void ATask8Character::BeginPlay()
{
	Super::BeginPlay();

	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);

		if (FireAbilityClass)
		{
			ASC->GiveAbility(FGameplayAbilitySpec(FireAbilityClass, 1, /*InputID*/0, this));
		}
	}
}

void ATask8Character::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ATask8Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced InputComponent로 캐스팅
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ATask8PlayerController* PlayerController = Cast<ATask8PlayerController>(GetController()))
		{
			if (UInputAction* JumpAction = PlayerController->GetJumpAction())
			{
				EnhancedInput->BindAction(
					JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ATask8Character::StartJump
				);

				EnhancedInput->BindAction(
					JumpAction,
					ETriggerEvent::Completed,
					this,
					&ATask8Character::StopJump
				);
			}

			if (UInputAction* SprintAction = PlayerController->GetSprintAction())
			{
				EnhancedInput->BindAction(
					SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ATask8Character::StartSprint
				);
				EnhancedInput->BindAction(
					SprintAction,
					ETriggerEvent::Completed,
					this,
					&ATask8Character::StopSprint
				);
			}

			if (UInputAction* FireAction = PlayerController->GetFireAction())
			{
				EnhancedInput->BindAction(
					FireAction,
					ETriggerEvent::Started,
					this,
					&ATask8Character::OnFirePressed
				);

			}
		}
	}
}

void ATask8Character::Landed(const FHitResult& Hit)
{
	bJumping = false;
}

void ATask8Character::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>() == true)
	{
		bJumping = true;
		Jump();
	}
}

void ATask8Character::StopJump(const FInputActionValue& value)
{
	if (value.Get<bool>() == false)
	{
		StopJumping();
	}
}

void ATask8Character::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement() &&
		bJumping == false)
	{
		bSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ATask8Character::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		bSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void ATask8Character::OnFirePressed()
{
	if (ASC && FireAbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Test Fire 1!"));
		ASC->TryActivateAbilityByClass(FireAbilityClass);
	}
}
