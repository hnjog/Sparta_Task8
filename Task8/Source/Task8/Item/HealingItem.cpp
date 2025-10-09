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
    if (HealEffectClass &&
        Activator &&
        Activator->ActorHasTag("Player"))
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player gained %d HP!"), HealAmount));

		if (UAbilitySystemComponent* ActASC =
			UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Activator))
		{
            FGameplayEffectContextHandle Ctx = ActASC->MakeEffectContext();

            const UGameplayEffect* GE = HealEffectClass->GetDefaultObject<UGameplayEffect>();

            ActASC->ApplyGameplayEffectToSelf(GE, 1.f, Ctx);
		}

        DestroyItem();
    }
}
