// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"
#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

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

void UMainMenuWidget::StartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), StartLevelName);
	//RemoveFromViewport();
}

void UMainMenuWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
