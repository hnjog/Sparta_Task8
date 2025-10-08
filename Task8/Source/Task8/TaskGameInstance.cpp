// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameInstance.h"


UTaskGameInstance::UTaskGameInstance()
{
	TotalScore = 0;
}

void UTaskGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}
