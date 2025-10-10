// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameState.h"
#include "SpawnVolume/EnemySpawnVolume.h"
#include "TaskGameInstance.h"
#include "Task8PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Item/ItemSpawnManager.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

ATaskGameState::ATaskGameState()
{
	LevelDuration = 20.0f; // 한 레벨당 30초
	SpawnEnemyDuration = 10.0f;
	CurrentLevelIndex = 0;
}

void ATaskGameState::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundItemManager;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemSpawnManager::StaticClass(), FoundItemManager);
	if (FoundItemManager.Num() > 0)
	{
		ItemSpawnManagerObj = Cast<AItemSpawnManager>(FoundItemManager[0]);
	}

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ATaskGameState::UpdateHUD,
		0.1f,
		true
	);
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
			TPC->ShowMainMenu(true);
		}
	}
}

void ATaskGameState::StartLevel()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ATask8PlayerController* TPC = Cast<ATask8PlayerController>(PC))
		{
			TPC->ShowGameHUD();
		}
	}

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawnVolume::StaticClass(), FoundVolumes);

	if (FoundVolumes.Num() > 0)
	{
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
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ATaskGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);

	UpdateHUD();
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

	StartLevel();
}

void ATaskGameState::UpdateHUD()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ATask8PlayerController* TPC = Cast<ATask8PlayerController>(PC))
		{
			if (UUserWidget* HUDWidget = TPC->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UTaskGameInstance* TGI = Cast<UTaskGameInstance>(GameInstance);
						if (TGI)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), TGI->GetTotalScore())));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}
			}
		}
	}
}
