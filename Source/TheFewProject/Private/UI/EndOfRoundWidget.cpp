// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EndOfRoundWidget.h"
#include "UI/ButtonWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UEndOfRoundWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (IsValid(MainMenuButton))
	{
		UButton* button = MainMenuButton->GetButton();
		if (IsValid(button))
			button->OnClicked.AddDynamic(this, &UEndOfRoundWidget::LoadMainMenu);
	}

	if (IsValid(QuitButton))
	{
		UButton* button = QuitButton->GetButton();
		if (IsValid(button))
			button->OnClicked.AddDynamic(this, &UEndOfRoundWidget::QuitGame);
	}
}

void UEndOfRoundWidget::LoadMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuName);
}

void UEndOfRoundWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
