// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Sets default values
AItemSpawnManager::AItemSpawnManager()
	:ItemDataTable(nullptr)
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
}

AActor* AItemSpawnManager::SpawnRandomItem(const FVector& PosVector)
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass,PosVector);
		}
	}

	return nullptr;
}


FItemSpawnRow* AItemSpawnManager::GetRandomItem() const
{
	if (!ItemDataTable)
		return nullptr;

	// 1) 모든 Row(행) 가져오기
	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext")); // 디버깅용 문자열
	ItemDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty())
		return nullptr;

	// 2) 전체 확률 합 구하기
	float TotalChance = 0.0f; // 초기화
	for (const FItemSpawnRow* Row : AllRows) // AllRows 배열의 각 Row를 순회
	{
		if (Row) // Row가 유효한지 확인
		{
			TotalChance += Row->SpawnChance; // SpawnChance 값을 TotalChance에 더하기
		}
	}

	// 3) 0 ~ TotalChance 사이 랜덤 값
	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;

	// 4) 누적 확률로 아이템 선택
	for (FItemSpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}

	return nullptr;
}

AActor* AItemSpawnManager::SpawnItem(TSubclassOf<AActor> ItemClass, const FVector& PosVector)
{
	if (!ItemClass) 
		return nullptr;

	return GetWorld()->SpawnActor<AActor>(
		ItemClass,
		PosVector,
		FRotator::ZeroRotator
	);
}
