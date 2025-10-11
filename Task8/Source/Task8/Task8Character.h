// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Character.h"
#include "Task8Character.generated.h"

struct FInputActionValue;

UCLASS(Blueprintable)
class ATask8Character : public ACharacter
{
	GENERATED_BODY()

public:
	ATask8Character();

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Landed(const FHitResult& Hit) override;

	virtual void Tick(float DeltaSeconds) override;

public:

	UFUNCTION()
	void Move(const FInputActionValue& value);

	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);

	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

public:
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintImplementableEvent)
	void SpeedEffectOrigin();

	UFUNCTION(BlueprintImplementableEvent)
	void SpeedEffectDecrease();

	void DecreaseSpeed(float decreaseSpeed, float decreaseDuration);

protected:
	void OnFirePressed();

	void OnHealthChanged(const FOnAttributeChangeData& Data);

	void Dead();

	void UpdateOverheadHP();

	void SetOriginSpeed();

	UFUNCTION()
	void OnAnyDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		AController* InstigatedBy, AActor* DamageCauser);

protected:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UWidgetComponent> OverheadWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr <class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UTaskAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<class UGameplayAbility> FireAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AActor> PistolClass;

	UPROPERTY() 
	TObjectPtr<AActor> EquippedPistol;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	float StartHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Death")
	float RagdollLifeTime = 3.0f;

	FTimerHandle SpeedTimerHandle;

	bool bJumping = false;
	bool bSprinting = false;
	bool bIsDead = false;
};

