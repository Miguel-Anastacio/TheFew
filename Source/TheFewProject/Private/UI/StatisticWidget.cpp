// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatisticWidget.h"
#include "Components/RichTextBlock.h"

void UStatisticWidget::Init(const FString& name, float value)
{
	if (Name)
	{
		Name->SetText(FText::FromString(name));
	}
	if (Value)
	{
		Value->SetText(FText::AsNumber(value));
	}

}
void UStatisticWidget::Init(const FString& name, int32 value)
{

	if (Name)
	{
		Name->SetText(FText::FromString(name));
	}
	if (Value)
	{
		Value->SetText(FText::AsNumber(value));
	}
}
void UStatisticWidget::UpdateValue(float value)
{
	if (Value)
		Value->SetText(FText::AsNumber(value));
}

void UStatisticWidget::UpdateValue(int32 value)
{
	if (Value)
		Value->SetText(FText::AsNumber(value));
}
