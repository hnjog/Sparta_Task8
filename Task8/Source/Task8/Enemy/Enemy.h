// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UAbilitySystemComponent;
class UTaskAttributeSet;
class UParticleSystem;

UCLASS()
class TASK8_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION()
    void OnHitGround(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:

    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void ResetSpeedToBase();
    void InitASCAndAttributes();
    void BindHealthChanged();

    void OnHealthChanged(const FOnAttributeChangeData& Data);

    UFUNCTION()
    void OnAnyDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
        AController* InstigatedBy, AActor* DamageCauser);

    // 자폭 트리거(플레이어와 부딪힘)
    UFUNCTION()
    void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    void Die(bool bFromSelfDestruct);
    void EnterRagdollAndScheduleDestroy();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
    TObjectPtr<UAbilitySystemComponent> AbilitySystem = nullptr;


    UPROPERTY()
    TObjectPtr<class UTaskAttributeSet> AttributeSet = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Move")
    float BaseSpeed = 250.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Move")
    float MaxSpeedUp = 800.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Move")
    float AccelPerSec = 150.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Damage")
    float TouchDamage = 40.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Death")
    float RagdollLifeTime = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|VFX")
    TObjectPtr<UParticleSystem> SelfDestructVFX = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Enemy|State")
    bool bSelfDestructed = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|State")
    bool bIsDead = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
    float StartHealth = 100.f;

    UPROPERTY()
    TObjectPtr<APawn> TargetPlayer = nullptr;
};
