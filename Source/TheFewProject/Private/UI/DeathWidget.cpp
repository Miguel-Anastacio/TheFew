// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DeathWidget.h"
#include "Components/RichTextBlock.h"

//#include "Components/Image.h"
//void UIndicatorWidget::SetIconTint(const FLinearColor& color)
//{
//	Icon->SetBrushTintColor(FSlateColor(color));
//}

void UDeathWidget::SetNameOfKiller(const FString& name)
{
	if (KillerName)
	{
		KillerName->SetText(FText::FromString(name));
	}
}

void UDeathWidget::ResetTimer()
{
	timer = Lifetime;
}

void UDeathWidget::NativeConstruct()
{
	Super::NativeConstruct();
	timer = Lifetime;
}

void UDeathWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	timer -= InDeltaTime;

	if (RespawnTimer)
	{
		int32 seconds = FMath::CeilToInt(timer);
		RespawnTimer->SetText(FText::AsNumber(seconds));
	}
}
