// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "WeaponBase.generated.h"

UCLASS()
class TASK8_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = "Weapon", meta = (DisplayName = "OnFire"))
    void K2_OnFire(
        const TArray<FVector>& ImpactPositions,
        const TArray<FVector>& ImpactNormals,
        const TArray<TEnumAsByte<EPhysicalSurface>>& SurfaceTypes
    );
};
