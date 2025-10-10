// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "DecreaseSpeedItem.generated.h"

/**
 * 
 */
UCLASS()
class TASK8_API ADecreaseSpeedItem : public ABaseItem
{
	GENERATED_BODY()
public:
	ADecreaseSpeedItem();

	virtual void ActivateItem(AActor* Activator) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float DecreaseSpeedAmount = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float DecreaseSpeedDuration = 10.0f;
};
