// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameState.h"
#include "SpawnVolume/EnemySpawnVolume.h"
#include "TaskGameInstance.h"
#include "Task8PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Item/ItemSpawnManager.h"

ATaskGameState::ATaskGameState()
{
	LevelDuration = 20.0f; // 한 레벨당 30초
	RestDuration = 5.0f; // 쉬는 시간 5초
	SpawnEnemyDuration = 10.0f;
	CurrentLevelIndex = 0;
}

void ATaskGameState::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundItemManager;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemSpawnManager::StaticClass(), FoundItemManager);
	ItemSpawnManagerObj = Cast<AItemSpawnManager>(FoundItemManager[0]);

	StartLevel();
}

void ATaskGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UTaskGameInstance* TaskGameI = Cast<UTaskGameInstance>(GameInstance))
		{
			TaskGameI->AddToScore(Amount);
		}
	}
}

void ATaskGameState::SpawnItem(const FVector& EnemyPos)
{
	if (ItemSpawnManagerObj)
	{
		ItemSpawnManagerObj->SpawnRandomItem(EnemyPos);
	}
}

void ATaskGameState::OnGameOver()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ATask8PlayerController* TPC = Cast<ATask8PlayerController>(PC))
		{
			TPC->SetPause(true);
		}
	}
}

void ATaskGameState::StartLevel()
{
	GetWorldTimerManager().ClearTimer(RestTimerHandle);

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawnVolume::StaticClass(), FoundVolumes);

	if (AEnemySpawnVolume* ESV = Cast<AEnemySpawnVolume>(FoundVolumes[0]))
	{
		float levelSpawnDuration = FMath::Clamp(SpawnEnemyDuration - (CurrentLevelIndex - 1), 0.5f, 20.0f);

		GetWorldTimerManager().SetTimer(
			SpawnEnemyTimerHandle,
			ESV,
			&AEnemySpawnVolume::SpawnEnemy,
			levelSpawnDuration,
			true,
			0.0f
		);
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ATaskGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);

}

void ATaskGameState::OnLevelTimeUp()
{
	EndLevel();
}

void ATaskGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	GetWorldTimerManager().ClearTimer(SpawnEnemyTimerHandle);
	CurrentLevelIndex++;

	GetWorldTimerManager().SetTimer(
		RestTimerHandle,
		this,
		&ATaskGameState::StartLevel,
		RestDuration,
		false
	);
}
