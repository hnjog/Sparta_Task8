// Copyright Epic Games, Inc. All Rights Reserved.

#include "Task8PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Task8Character.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "TaskGameState.h"
#include "TaskGameInstance.h"
#include "Kismet/GameplayStatics.h"

ATask8PlayerController::ATask8PlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ATask8PlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuMap"))
	{
		ShowMainMenu(false);
	}
}

UUserWidget* ATask8PlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

// Possess 이후, InputComponent 생성 시점에 호출
void ATask8PlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ATask8PlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());

			ATaskGameState* TGS = GetWorld() ? GetWorld()->GetGameState<ATaskGameState>() : nullptr;
			if (TGS)
			{
				TGS->UpdateHUD();
			}
		}
	}
}

void ATask8PlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("Restart")));
			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("Start")));
			}
		}

		if (bIsRestart)
		{
			UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}

			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (UTaskGameInstance* TGI = Cast<UTaskGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(
						FString::Printf(TEXT("Total Score: %d"), TGI->GetTotalScore())
					));
				}
			}
		}
	}
}

void ATask8PlayerController::StartGame()
{
	if (UTaskGameInstance* TGI = Cast<UTaskGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		TGI->ResetTotalScore();
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("TopDownMap"));
	SetPause(false);
}
