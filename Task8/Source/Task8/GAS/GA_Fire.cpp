// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Fire.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"

UGA_Fire::UGA_Fire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Info, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 비용 적용, 쿨다운 처리 등을 위하여 호출
	//
	CommitAbility(Handle, Info, ActivationInfo);

    UE_LOG(LogTemp, Warning, TEXT("Test Fire 2!"));

	AActor* Avatar = Info ? Info->AvatarActor.Get() : nullptr;
	if (Avatar == nullptr)
	{
		EndAbility(Handle, Info, ActivationInfo, true, false);
		return;
	}

    FVector Start = Avatar->GetActorLocation();
    FVector Dir = Avatar->GetActorForwardVector(); // 기본값
    // 캐릭터면 카메라/Eye 또는 소켓 기준
    if (ACharacter* C = Cast<ACharacter>(Avatar))
    {
        if (USkeletalMeshComponent* Mesh = C->GetMesh())
        {
            if (Mesh->DoesSocketExist(MuzzleSocketName))
                Start = Mesh->GetSocketLocation(MuzzleSocketName);
        }
    }

    FVector End = Start + Dir * TraceDistance;

    FHitResult Hit;
    FCollisionQueryParams P(SCENE_QUERY_STAT(GA_Fire), /*bTraceComplex=*/true);
    P.AddIgnoredActor(Avatar);

    bool bHit = Avatar->GetWorld()->LineTraceSingleByChannel(
        Hit, Start, End, ECC_Visibility, P);

    // 디버그 라인(선택)
    // DrawDebugLine(Avatar->GetWorld(), Start, bHit ? Hit.ImpactPoint : End, FColor::Red, false, 1.f, 0, 1.f);

    if (bHit && DamageEffectClass)
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

    EndAbility(Handle, Info, ActivationInfo, true, false);
}
