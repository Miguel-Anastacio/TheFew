// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndOfRoundWidget.generated.h"

/**
 * 
 */
UCLASS()
class UEndOfRoundWidget : public UUserWidget
{
	GENERATED_BODY()
public:
//	//void SetNameOfKiller(const FString& name);
	UPROPERTY(EditAnywhere)
		float Lifetime = 5.0f;
	//void ResetTimer();
	//void PlayExitAnimation();
protected:
	//virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION()
		void LoadMainMenu();
	UFUNCTION()
		void QuitGame();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButtonWidget> MainMenuButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButtonWidget> QuitButton;

	UPROPERTY(EditAnywhere, Category = "Start Level")
	FName MainMenuName;

};
