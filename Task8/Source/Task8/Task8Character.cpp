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

	GetCharacterMovement()->bOrientRotationToMovement = false;
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
	}

	if (PistolClass && GetMesh())
	{
		FActorSpawnParameters P; 
		P.Owner = this; 
		P.Instigator = this;
		EquippedPistol = GetWorld()->SpawnActor<AActor>(PistolClass, GetActorTransform(), P);
		if (EquippedPistol)
		{
			USkeletalMeshComponent* WeaponMesh = EquippedPistol->FindComponentByClass<USkeletalMeshComponent>();
			WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			EquippedPistol->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				TEXT("weapon_r")); // 캐릭터 소켓명
			EquippedPistol->GetRootComponent()->SetRelativeRotation(FRotator(0, -100, 0));
		}
	}

	if (ASC && FireAbilityClass && EquippedPistol)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(FireAbilityClass, 1, /*InputID*/0, EquippedPistol));
	}
}

void ATask8Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC == nullptr)
		return;

	// 커서 아래 충돌 검사 (채널은 프로젝트에 맞춰 수정: ECC_Visibility 등)
	FHitResult Hit;
	const bool bHit = PC->GetHitResultUnderCursor(ECC_Visibility, /*bTraceComplex=*/false, Hit);
	if (!bHit)
		return;

	// 캐릭터에서 커서 지점까지 2D 방향(Yaw만)
	FVector To = Hit.ImpactPoint - GetActorLocation();
	To.Z = 0.f;

	if (!To.IsNearlyZero())
	{
		const FRotator DesiredRot = To.Rotation();
		const FRotator TargetRot(0.f, DesiredRot.Yaw, 0.f);
		const FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaSeconds, 15.f); // 15는 회전 속도(보간)
		SetActorRotation(NewRot);
	}
}

void ATask8Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced InputComponent로 캐스팅
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ATask8PlayerController* PlayerController = Cast<ATask8PlayerController>(GetController()))
		{
			if (UInputAction* MoveAction = PlayerController->GetMoveAction())
			{
				// IA_Move 액션 키를 "키를 누르고 있는 동안" Move() 호출
				EnhancedInput->BindAction(
					MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ATask8Character::Move
				);
			}

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

void ATask8Character::Move(const FInputActionValue& value)
{
	if (Controller == nullptr)
		return;

	const FVector2D MoveValue = value.Get<FVector2D>();

	const float YValue = GetControlRotation().Yaw;
	const FRotator YawRot(0.f, YValue, 0.f);

	if (FMath::IsNearlyZero(MoveValue.X) == false)
	{
		const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		AddMovementInput(Forward, MoveValue.X);
	}

	if (FMath::IsNearlyZero(MoveValue.Y) == false)
	{
		const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
		AddMovementInput(Right, MoveValue.Y);
	}
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
	if (bJumping ||
		bSprinting)
		return;

	if (ASC && FireAbilityClass)
	{
		ASC->TryActivateAbilityByClass(FireAbilityClass);
	}
}
