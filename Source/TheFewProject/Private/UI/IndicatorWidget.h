// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IndicatorWidget.generated.h"

/**
 * 
 */
UCLASS()
class UIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//void SetIconTint(const FLinearColor& color);
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;
	//UPROPERTY(meta = (BindWidget))
	//class UImage* Background;

};
