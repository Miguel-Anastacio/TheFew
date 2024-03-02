// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlaneHUD.generated.h"

/**
 * 
 */
UCLASS()
class UPlaneHUD : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetPlaneReference(class APlanePawnPlayer* ref);
	void ToggleScoreboard(bool status);
	void DisplayDeathScreen();

	void DisplayOutOfBoundsWidget();
	void RemoveOutOfBoundsWidget();
	float GetLifetimeOfOutOfBoundsWidget();
	UFUNCTION()
	void DisplaySpawnScreen();
	void RemoveSpawnScreen();

	UFUNCTION()
	void UpdateHealthBar(float currentPercent);

	void FocusActiveWidget();
	UFUNCTION(BlueprintCallable)
		void SetActiveWidget(UUserWidget* widget);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void DisplayHitmarker();
	UFUNCTION()
	void HideHitmarker();
	UFUNCTION()
	void RemoveDeathScreen();

	UFUNCTION()
		void DisplayEndOfRound();



protected:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ThrottleBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> CrosshairImageClass;
	UPROPERTY(meta = (BindWidget))
		class URichTextBlock* AltitudeValue;
	UPROPERTY(meta = (BindWidget))
		class URichTextBlock* VelocityValue;
	UPROPERTY(meta = (BindWidget))
		class UImage* HitmarkerIcon;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTotalScoreWidget> TotalScoreWidget;

	// Game Timer
	UPROPERTY(meta = (BindWidget))
		class URichTextBlock* MinutesText;
	UPROPERTY(meta = (BindWidget))
		class URichTextBlock* SecondsText;

	TWeakObjectPtr<class AArenaGameState> GameState;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UScoreboardWidget> ScoreboardClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UDeathWidget> DeathClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UOutOfBoundsWidget> OutOfBoundsClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class USpawnMenuWidget> SpawnClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UEndOfRoundWidget> EndOfRoundClass;
	UPROPERTY(EditAnywhere, Category = "Hitmarker")
		float DurationHitmarker = 0.3f;

	TWeakObjectPtr<class APlanePawnPlayer> ControlledPlane;
	float* ThrottleRef = NULL;
	UUserWidget* CrosshairWidget = NULL;
	TObjectPtr<class UScoreboardWidget> ScoreboardWidget;
	TObjectPtr<class UDeathWidget> DeathWidget;
	TObjectPtr<class UOutOfBoundsWidget> OutOfBoundsWidget;
	TObjectPtr<class USpawnMenuWidget> SpawnMenuWidget;
	TObjectPtr<class UEndOfRoundWidget> EndOfRoundWidget;

	TWeakObjectPtr<UUserWidget> ActiveWidget;

	FTimerHandle DeathTimerHandle;
	// health bar
	
};
