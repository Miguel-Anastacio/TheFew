// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/ButtonWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	//virtual void NativeConstruct() override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnInitialized() override;

	UFUNCTION()
		void StartGame();
	
	UFUNCTION()
		void QuitGame();
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		TObjectPtr<UButtonWidget> StartButton;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButtonWidget> QuitButton;

	UPROPERTY(EditAnywhere, Category = "Start Level")
	FName StartLevelName;
};
