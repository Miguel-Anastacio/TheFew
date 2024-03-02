// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"
#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/UObjectGlobals.h"
#include "Components/CircularThrobber.h"
void UMainMenuWidget::AsyncLevelLoad(const FString& levelDir, const FString& levelName)
{
	LoadPackageAsync(levelDir + levelName,
		FLoadPackageAsyncDelegate::CreateLambda([=, this](const FName& packageName, UPackage* loadedPackage, EAsyncLoadingResult::Type result)
			{
				if (result == EAsyncLoadingResult::Succeeded)
				{
					AsyncLevelLoadFinished(levelName);
				}
			}
		),
		0,
				PKG_ContainsMap);
	if (LoadIndicator)
	{
		LoadIndicator->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	if(StartButton)
		StartButton->SetVisibility(ESlateVisibility::Collapsed);
	if(QuitButton)
		QuitButton->SetVisibility(ESlateVisibility::Collapsed);
	//PlayAnimation(LoadingAnim, 0.0f, 100, EUMGSequencePlayMode::Forward, 1.0f, true);
}

void UMainMenuWidget::OnPreheatFinished()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
}

void UMainMenuWidget::NativeOnInitialized()
{
	if (IsValid(StartButton))
	{
		UButton* button = StartButton->GetButton();
		if (IsValid(button))
			button->OnClicked.AddDynamic(this, &UMainMenuWidget::StartGame);
	}

	if (IsValid(QuitButton))
	{
		UButton* button = QuitButton->GetButton();
		if(IsValid(button))
			button->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
	}
	
}

void UMainMenuWidget::AsyncLevelLoadFinished(const FString& levelName)
{
	FTimerHandle timer;
	LevelName = levelName;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UMainMenuWidget::OnPreheatFinished, 3.0f, false);
}


void UMainMenuWidget::StartGame()
{
	AsyncLevelLoad(LevelDirectory, StartLevelName);

	//RemoveFromViewport();
}

void UMainMenuWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
