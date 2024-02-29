// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatisticWidget.generated.h"

/**
 * 
 */
UCLASS()
class UStatisticWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(const FString& name, float value);
	void Init(const FString& name, int32 value);

	void UpdateValue(float value);
	void UpdateValue(int32 value);

	UPROPERTY(EditAnywhere)
		FText NameText;
	UPROPERTY(EditAnywhere)
		FText ValueText;

protected:

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class URichTextBlock> Name;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class URichTextBlock> Value;


	//int teamAID = 0;
	//int teamBID = 0;


};
