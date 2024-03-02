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
public:
	UFUNCTION(BlueprintCallable)
		void AsyncLevelLoad(const FString& levelDir, const FString& levelName);
protected:
	UFUNCTION()
		void OnPreheatFinished();
	//virtual void NativeConstruct() override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnInitialized() override;
	void AsyncLevelLoadFinished(const FString& LevelName);
	//void PlayLoadAnimation();
	UFUNCTION()
		void StartGame();
	UFUNCTION()
		void QuitGame();
	UPROPERTY(EditAnywhere, meta = (BindWidget), BlueprintReadWrite)
		TObjectPtr<UButtonWidget> StartButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		TObjectPtr<UButtonWidget> QuitButton;


	UPROPERTY(EditAnywhere, Category = "Start Level")
	FString StartLevelName;
	UPROPERTY(EditAnywhere, Category = "Start Level")
	FString LevelDirectory;

	FString LevelName;
	UPROPERTY(meta = (BindWiget))
	class UCircularThrobber* LoadIndicator;

};
