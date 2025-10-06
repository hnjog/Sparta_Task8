// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Fire.generated.h"

/**
 * 
 */
UCLASS()
class TASK8_API UGA_Fire : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Fire();

    // 데미지 효과 (BP에서 GE_Damage 지정)
    UPROPERTY(EditDefaultsOnly, Category = "Fire")
    TSubclassOf<class UGameplayEffect> DamageEffectClass;

    // 총구 소켓명 (없으면 카메라 기준)
    UPROPERTY(EditDefaultsOnly, Category = "Fire")
    FName MuzzleSocketName = "Muzzle";

    UPROPERTY(EditDefaultsOnly, Category = "Fire")
    float TraceDistance = 10000.f;

    UPROPERTY(EditDefaultsOnly) 
    float FireInterval = 0.1f;

    FTimerHandle FireTimer;
    bool bCanFire = true;

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* Info,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

};
