// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

/**
 *
 */
UCLASS()
class TASK8_API AMineItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AMineItem();

	virtual void ActivateItem(AActor* Activator) override;

	void Explode();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	TObjectPtr<USphereComponent> ExplosionCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	TObjectPtr<UParticleSystem> ExplosionParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	TObjectPtr<USoundBase> ExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float ExplosionDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float ExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float ExplosionDamage;

	FTimerHandle ExplosionTimerHandle;
	bool bHasExploded;
};
