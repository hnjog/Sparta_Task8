// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

AEnemySpawnVolume::AEnemySpawnVolume()
{
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
    SpawningBox->SetupAttachment(Scene);
}

FVector AEnemySpawnVolume::GetRandomPointInVolume() const
{
    FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
    FVector BoxOrigin = SpawningBox->GetComponentLocation();

    return BoxOrigin + FVector(
        FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
        FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
        FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
    );
}

AActor* AEnemySpawnVolume::SpawnEnemyRandomPos()
{
    if (!EnemyClass)
        return nullptr;

    return GetWorld()->SpawnActor<AActor>(
        EnemyClass,
        GetRandomPointInVolume(),
        FRotator::ZeroRotator
    );
}


void AEnemySpawnVolume::SpawnEnemy()
{
    SpawnEnemyRandomPos();
}