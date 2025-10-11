// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskAttributeSet.h"
#include "GameplayEffectExtension.h"

UTaskAttributeSet::UTaskAttributeSet()
{
	Health.SetBaseValue(100.f);
	Health.SetCurrentValue(100.f);
	MaxHealth.SetBaseValue(100.f);
	MaxHealth.SetCurrentValue(100.f);
}

void UTaskAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const float Clamped = FMath::Clamp(GetHealth(), 0.f, GetMaxHealth());
		SetHealth(Clamped);
	}
}
