// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class TASK8_API AEnemySpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnVolume();

	FVector GetRandomPointInVolume() const;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnEnemyRandomPos();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnEnemy();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<UBoxComponent> SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AActor> EnemyClass;
};
