// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OutOfBoundsWidget.h"
#include "Components/RichTextBlock.h"

//#include "Components/Image.h"
//void UIndicatorWidget::SetIconTint(const FLinearColor& color)
//{
//	Icon->SetBrushTintColor(FSlateColor(color));
//}

//void UOutOfBoundsWidget::SetNameOfKiller(const FString& name)
//{
//	if (KillerName)
//	{
//		KillerName->SetText(FText::FromString(name));
//	}
//}
//
void UOutOfBoundsWidget::ResetTimer()
{
	timer = Lifetime;
}

void UOutOfBoundsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	timer = Lifetime;
	EndDelegate.BindDynamic(this, &UOutOfBoundsWidget::AnimationFinished);

	BindToAnimationFinished(FadeAwayAnim, EndDelegate);
}

void UOutOfBoundsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	timer -= InDeltaTime;

	if (RespawnTimer)
	{
		int32 seconds = FMath::CeilToInt(timer);
		RespawnTimer->SetText(FText::AsNumber(seconds));
	}
}

void UOutOfBoundsWidget::PlayExitAnimation()
{
	PlayAnimation(FadeAwayAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, true);
}

void UOutOfBoundsWidget::AnimationFinished()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}
