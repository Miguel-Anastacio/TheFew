// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ButtonWidget.h"
#include "Components/RichTextBlock.h"
void UButtonWidget::NativePreConstruct()
{
	if (IsValid(ButtonText))
	{
		ButtonText->SetText(Text);
	}
}
