// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "Task8PlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class UUserWidget;

UCLASS()
class ATask8PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATask8PlayerController();

	FORCEINLINE UInputAction* GetMoveAction() const { return MoveAction; }
	FORCEINLINE UInputAction* GetJumpAction() const { return JumpAction; }
	FORCEINLINE UInputAction* GetSprintAction() const { return SprintAction; }
	FORCEINLINE UInputAction* GetFireAction() const { return FireAction; }
protected:
	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
	// 메인 메뉴 표시
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu(bool bIsRestart);
	// 게임 시작
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();

protected:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	TObjectPtr<UUserWidget> HUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	TObjectPtr<UUserWidget> MainMenuWidgetInstance;
};


