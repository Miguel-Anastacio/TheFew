// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/UObjectGlobals.h"
#include "Components/CircularThrobber.h"
void UGameMainMenuWidget::AsyncLevelLoad(const FString& levelDir, const FString& levelName)
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

void UGameMainMenuWidget::OnPreheatFinished()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
}

void UGameMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (IsValid(StartButton))
	{
		UButton* button = StartButton->GetButton();
		if (IsValid(button))
			button->OnClicked.AddDynamic(this, &UGameMainMenuWidget::StartGame);
		
		StartButton->UIInputActionDelegate.AddDynamic(this, &UGameMainMenuWidget::StartGame);
	}

	if (IsValid(QuitButton))
	{
		UButton* button = QuitButton->GetButton();
		if(IsValid(button))
			button->OnClicked.AddDynamic(this, &UGameMainMenuWidget::QuitGame);

		QuitButton->UIInputActionDelegate.AddDynamic(this, &UGameMainMenuWidget::QuitGame);
	}
	
}

void UGameMainMenuWidget::AsyncLevelLoadFinished(const FString& levelName)
{
	FTimerHandle timer;
	LevelName = levelName;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UGameMainMenuWidget::OnPreheatFinished, 3.0f, false);
}


void UGameMainMenuWidget::StartGame()
{
	AsyncLevelLoad(LevelDirectory, StartLevelName);

	//RemoveFromViewport();
}

void UGameMainMenuWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
