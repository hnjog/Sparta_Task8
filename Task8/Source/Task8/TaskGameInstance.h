// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TaskGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TASK8_API UTaskGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTaskGameInstance();

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);

	FORCEINLINE int32 GetTotalScore() { return TotalScore; }
	FORCEINLINE void ResetTotalScore() { TotalScore = 0; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
};
