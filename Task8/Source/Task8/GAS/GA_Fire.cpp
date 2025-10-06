// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Fire.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "../Weapon/WeaponBase.h"

UGA_Fire::UGA_Fire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Info, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (bCanFire == false)
	{
		EndAbility(Handle, Info, ActivationInfo, true, false);
		return;
	}

	bCanFire = false;

	if (CommitAbility(Handle, Info, ActivationInfo) == false)
	{
		EndAbility(Handle, Info, ActivationInfo, true, false);
		return;
	}

	AActor* Avatar = Info ? Info->AvatarActor.Get() : nullptr;
	if (Avatar == nullptr)
	{
		EndAbility(Handle, Info, ActivationInfo, true, false);
		return;
	}

	TArray<FVector> Points, Normals;
	TArray<TEnumAsByte<EPhysicalSurface>> Surfaces;

	FVector Start = Avatar->GetActorLocation();
	FVector Dir = Avatar->GetActorForwardVector(); // 기본값

	Points.Add(Start);
	Normals.Add(FVector::ZeroVector);
	Surfaces.Add(SurfaceType_Default);

	// 캐릭터면 카메라/Eye 또는 소켓 기준
	if (ACharacter* C = Cast<ACharacter>(Avatar))
	{
		if (USkeletalMeshComponent* Mesh = C->GetMesh())
		{
			if (Mesh->DoesSocketExist(MuzzleSocketName))
			{
				Start = Mesh->GetSocketLocation(MuzzleSocketName);
				if (AActor* Weapon = Cast<AActor>(GetSourceObject(Handle, Info)))
					Dir = Weapon->GetActorForwardVector();
			}
		}
	}

	FVector End = Start + Dir * TraceDistance;

	FHitResult Hit;
	FCollisionQueryParams P(SCENE_QUERY_STAT(GA_Fire), /*bTraceComplex=*/true);
	P.AddIgnoredActor(Avatar);

	bool bHit = Avatar->GetWorld()->LineTraceSingleByChannel(
		Hit, Start, End, ECC_Visibility, P);

	if (bHit)
	{
		Points.Add(Hit.ImpactPoint);
		Normals.Add(Hit.ImpactNormal);
		Surfaces.Add(UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get()));

		if (DamageEffectClass)
		{
			if (UAbilitySystemComponent* TargetASC =
				UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Hit.GetActor()))
			{
				FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
				if (Spec.IsValid())
				{
					TargetASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
				}
			}
		}
	}

	if (AWeaponBase* Weapon = Cast<AWeaponBase>(GetSourceObject(Handle, Info)))
	{
		Weapon->K2_OnFire(Points, Normals, Surfaces);
	}

	GetWorld()->GetTimerManager().SetTimer(FireTimer, [this]()
		{
			bCanFire = true;
		}, FireInterval, false);

	EndAbility(Handle, Info, ActivationInfo, true, false);
}
