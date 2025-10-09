// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "../GAS/TaskAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Characters/Robot_scout_R_21/Mesh/SK_Robot_scout_R21.SK_Robot_scout_R21"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -90.0));
		GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

		// Anim Class를 찾아 세팅
		// Class를 찾는것이기에 마지막에 _C 를 붙인다!
		ConstructorHelpers::FClassFinder<UAnimInstance> MeshAnimAsset(TEXT("/Game/Characters/Robot_scout_R_21/Demo/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C"));
		if (MeshAnimAsset.Succeeded())
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(MeshAnimAsset.Class);
		}
	}

	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UTaskAttributeSet>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	ResetSpeedToBase();

	TargetPlayer = UGameplayStatics::GetPlayerPawn(this, 0);

	InitASCAndAttributes();
	BindHealthChanged();

	OnTakeAnyDamage.AddDynamic(this, &AEnemy::OnAnyDamageTaken);

	// 자폭 트리거
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionProfileName(TEXT("Pawn"));
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		Capsule->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnCapsuleBeginOverlap);
	}

	if (Controller == nullptr)
	{
		SpawnDefaultController();
	}
}

void AEnemy::InitASCAndAttributes()
{
	if (!AbilitySystem)
		return;

	AbilitySystem->InitAbilityActorInfo(this, this);

	if (AbilitySystem)
	{
		const FGameplayAttribute HealthAttr = UTaskAttributeSet::GetHealthAttribute();
		AbilitySystem->SetNumericAttributeBase(HealthAttr, StartHealth);
	}
}

void AEnemy::BindHealthChanged()
{
	if (!AbilitySystem)
		return;

	const FGameplayAttribute HealthAttr = UTaskAttributeSet::GetHealthAttribute();
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(HealthAttr).AddUObject(this, &AEnemy::OnHealthChanged);
}

void AEnemy::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (bIsDead)
		return;

	const float NewHealth = Data.NewValue;

	UE_LOG(LogTemp, Warning, TEXT("Enemy Now Hp : %f"), NewHealth);
	UE_LOG(LogTemp, Warning, TEXT("Enemy Prev Hp : %f"), Data.OldValue);

	if (NewHealth < Data.OldValue)
	{
		ResetSpeedToBase();
	}

	if (NewHealth <= 0.f)
	{
		Die(false);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDead ||
		GetCharacterMovement()->IsFalling())
		return;

	if (!TargetPlayer)
	{
		TargetPlayer = UGameplayStatics::GetPlayerPawn(this, 0);
	}

	float NewSpeed = FMath::Min(GetCharacterMovement()->MaxWalkSpeed + AccelPerSec * DeltaTime, MaxSpeedUp);
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

	if (TargetPlayer)
	{
		const FVector ToPlayer = TargetPlayer->GetActorLocation() - GetActorLocation();
		const FRotator Look = ToPlayer.Rotation();
		SetActorRotation(FRotator(0, Look.Yaw, 0));

		AddMovementInput(GetActorForwardVector(), 1.f);
	}
}

void AEnemy::ResetSpeedToBase()
{
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
}

void AEnemy::OnAnyDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsDead || Damage <= 0.f || !AbilitySystem)
		return;

	const FGameplayAttribute HealthAttr = UTaskAttributeSet::GetHealthAttribute();
	AbilitySystem->ApplyModToAttributeUnsafe(HealthAttr, EGameplayModOp::Additive, -Damage);
}

void AEnemy::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsDead || bSelfDestructed)
		return;
	if (!OtherActor)
		return;

	if (OtherActor == TargetPlayer)
	{
		bSelfDestructed = true;

		if (SelfDestructVFX)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelfDestructVFX,
				GetActorLocation(), GetActorRotation());
		}

		UGameplayStatics::ApplyDamage(OtherActor, TouchDamage, GetController(), this, nullptr);

		Die(true);
	}
}

void AEnemy::Die(bool bFromSelfDestruct)
{
	if (bIsDead)
		return;
	bIsDead = true;

	GetCharacterMovement()->DisableMovement();
	if (Controller)
	{
		Controller->StopMovement();
	}

	DetachFromControllerPendingDestroy();

	EnterRagdollAndScheduleDestroy();
}

void AEnemy::EnterRagdollAndScheduleDestroy()
{
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
		MeshComp->SetSimulatePhysics(true);
		MeshComp->WakeAllRigidBodies();
		MeshComp->bBlendPhysics = true;
	}

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	SetLifeSpan(RagdollLifeTime);
}