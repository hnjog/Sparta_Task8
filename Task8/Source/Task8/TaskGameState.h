// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TaskGameState.generated.h"

/**
 * 
 */
UCLASS()
class TASK8_API ATaskGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ATaskGameState();

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SpawnItem(const FVector& EnemyPos);

	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	void StartLevel();

	void OnLevelTimeUp();

	void EndLevel();

	void UpdateHUD();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	float SpawnEnemyDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<class AItemSpawnManager> ItemSpawnManagerObj;

	FTimerHandle LevelTimerHandle;
	FTimerHandle SpawnEnemyTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;
};
