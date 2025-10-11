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

void UTaskAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		const float MaxHP = GetMaxHealth();
		NewValue = FMath::Clamp(NewValue, 0.f, MaxHP);
	}
}
