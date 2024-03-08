// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameButtonWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/Button.h"
void UGameButtonWidget::NativePreConstruct()
{
	if (IsValid(ButtonText))
	{
		ButtonText->SetText(Text);
	}
}

