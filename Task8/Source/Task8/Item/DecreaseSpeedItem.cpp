// Fill out your copyright notice in the Description page of Project Settings.


#include "DecreaseSpeedItem.h"
#include "../Task8Character.h"

ADecreaseSpeedItem::ADecreaseSpeedItem()
{
	ItemType = "DecreaseSpeedItem";
}

void ADecreaseSpeedItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator &&
		Activator->ActorHasTag("Player"))
	{
		if (ATask8Character* TC = Cast< ATask8Character>(Activator))
		{
			TC->DecreaseSpeed(DecreaseSpeedAmount, DecreaseSpeedDuration);
		}

		DestroyItem();
	}
}
