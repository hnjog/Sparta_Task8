// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"
#include "../TaskGameState.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);
    if (Activator && Activator->ActorHasTag("Player"))
    {
        // 점수 획득 디버그 메시지
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player gained %d points!"), PointValue));

        if (UWorld* World = GetWorld())
        {
            if (ATaskGameState* GameState = World->GetGameState<ATaskGameState>())
            {
                GameState->AddScore(PointValue);
            }
        }

        // 부모 클래스 (BaseItem)에 정의된 아이템 파괴 함수 호출
        DestroyItem();
    }
}
