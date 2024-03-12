// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LayerManagerHUD.h"
#include "ManagerHUD.generated.h"

/**
 * 
 */
UCLASS()
class AManagerHUD : public ALayerManagerHUD
{
	GENERATED_BODY()

public:
	void Init();

	void DisplayHUD();
	void UpdateWidgetHUD(int32 healthBar);

	UFUNCTION(BlueprintNativeEvent, Category = "UI Event")
	void ToggleScoreboard(bool status);
	void ToggleScoreboard_Implementation(bool status);

	// You can define the pause behaviour either in blueprint or c++
	UFUNCTION(BlueprintNativeEvent, Category = "UI Event")
	void TogglePause();
	void TogglePause_Implementation();

	//void SetPlaneReference(class APlanePawn* plane);
	void ToggleWidget(UUserWidget* widget, const FString& layer);

	void DisplayDeathScreen();
	UFUNCTION()
	void RemoveDeathScreen();

	void DisplayOutOfBoundsWidget();
	void RemoveOutOfBoundsWidget();
	float GetLifetimeOfOutOfBoundsWidget() const;

	UFUNCTION()
	void DisplaySpawnScreen();
	UFUNCTION()
	void DisplayEndOfRound();

	void FocusActiveWidget() const;
	void UnFocusActiveWidget() const;
	UFUNCTION(BlueprintCallable)
	void SetActiveWidget(UUserWidget* widget);
	//float GetLifetimeOfOutOfBoundsWidget();
	//UFUNCTION()
	//void DisplaySpawnScreen();
	//void RemoveSpawnScreen();

	void PopFromLayerWithFocus(const FString& layer);

	

public:
	UPROPERTY(EditAnywhere, Category = "Layers | Tags")
	FString Game;
	UPROPERTY(EditAnywhere, Category = "Layers | Tags")
	FString GameMenu;
	UPROPERTY(EditAnywhere, Category = "Layers | Tags")
	FString Priority;
	UPROPERTY(EditAnywhere, Category = "Layers | Tags")
	FString Menu;

	//void TogglePauseMenu();

	//void FocusActiveWidget();
	//void UnFocusActiveWidget();
	//UFUNCTION(BlueprintCallable)
	//void SetActiveWidget(UUserWidget* widget);

protected:
	UPROPERTY(EditAnywhere, Category = "Layers | Custom")
	TSubclassOf<class ULayerUI> PrioLayer;
	UPROPERTY(EditAnywhere, Category = "Layers | Custom")
	TSubclassOf<class ULayerUI> GameLayer;

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
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UEndOfRoundWidget> PauseMenuClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlaneHUD> HUDClass;

	TObjectPtr<class UScoreboardWidget> ScoreboardWidget;
	TObjectPtr<class UDeathWidget> DeathWidget;
	TObjectPtr<class UOutOfBoundsWidget> OutOfBoundsWidget;
	TObjectPtr<class USpawnMenuWidget> SpawnMenuWidget;
	TObjectPtr<class UEndOfRoundWidget> EndOfRoundWidget;
	TObjectPtr<class UEndOfRoundWidget> PauseMenuWidget;
	TObjectPtr<class UPlaneHUD> HUDWidget;
	TWeakObjectPtr<UUserWidget> ActiveWidget;

	//UPROPERTY(BlueprintReadWrite)
	//TObjectPtr<class UUserWidget> PauseWidget;
	
};
