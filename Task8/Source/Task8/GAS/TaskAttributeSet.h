// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TaskAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName) \
 GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) \

/**
 * 
 */
UCLASS()
class TASK8_API UTaskAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UTaskAttributeSet();

	ATTRIBUTE_ACCESSORS(UTaskAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(UTaskAttributeSet, MaxHealth)

protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attr")
	FGameplayAttributeData Health;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attr")
	FGameplayAttributeData MaxHealth;
};
