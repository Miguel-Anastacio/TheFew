// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerScoreWidget.h"
#include "Components/RichTextBlock.h"
//void UPlayerScoreWidget::NativePreConstruct()
//{
//	if (IsValid(ButtonText))
//	{
//		ButtonText->SetText(Text);
//	}
//}

void UPlayerScoreWidget::Init(const FString& name, int32 kills, int32 deaths, int32 score)
{
	if (PlayerName)
	{
		PlayerName->SetText(FText::FromString(name));
	}
	if (Kills)
	{
		Kills->SetText(FText::AsNumber(kills));
	}
	if (Deaths)
	{
		Deaths->SetText(FText::AsNumber(deaths));
	}
	if (Score)
	{
		Score->SetText(FText::AsNumber(score));
	}
}

void UPlayerScoreWidget::Update(int32 kills, int32 deaths, int32 score)
{
	if (Kills)
	{
		Kills->SetText(FText::AsNumber(kills));
	}
	if (Deaths)
	{
		Deaths->SetText(FText::AsNumber(deaths));
	}
	if (Score)
	{
		Score->SetText(FText::AsNumber(score));
	}
}
