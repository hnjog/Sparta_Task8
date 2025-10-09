// Fill out your copyright notice in the Description page of Project Settings.

#include "HealingItem.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);
    //if (Activator && Activator->ActorHasTag("Player"))
    //{
    //    // 점수 획득 디버그 메시지
    //    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player gained %d HP!"), HealAmount));

    //    if (ATaskPlayer* Player = Cast<ATaskPlayer>(Activator))
    //    {
    //        Player->HealHealth(HealAmount);
    //    }

    //    // 부모 클래스 (BaseItem)에 정의된 아이템 파괴 함수 호출
    //    DestroyItem();
    //}
}
