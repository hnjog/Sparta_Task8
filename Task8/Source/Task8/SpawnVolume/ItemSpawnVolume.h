// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../DataTable/ItemSpawnRow.h"
#include "ItemSpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class TASK8_API AItemSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawnVolume();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomItem();

	FVector GetRandomPointInVolume() const;
	FItemSpawnRow* GetRandomItem() const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<UBoxComponent> SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<UDataTable> ItemDataTable;
};
