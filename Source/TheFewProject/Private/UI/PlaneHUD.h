// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlaneHUD.generated.h"

/**
 * 
 */
class APlanePawn;
UCLASS()
class UPlaneHUD : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetPlaneReference(APlanePawn* ref);
	void ToggleScoreboard(bool status);
	void DisplayDeathScreen();
	UFUNCTION()
	void UpdateHealthBar(float currentPercent);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void RemoveDeathScreen();
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ThrottleBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> CrosshairImageClass;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AltitudeValue;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UScoreboardWidget> ScoreboardClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UDeathWidget> DeathClass;

	APlanePawn* ControlledPlane;
	float* ThrottleRef = NULL;
	UUserWidget* CrosshairWidget = NULL;
	TObjectPtr<class UScoreboardWidget> ScoreboardWidget;
	TObjectPtr<class UDeathWidget> DeathWidget;
	// health bar
	
};
