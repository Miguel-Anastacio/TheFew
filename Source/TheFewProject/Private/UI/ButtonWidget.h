// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class UButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	class UButton* GetButton() { return Button; };
	virtual void NativePreConstruct() override;

	UPROPERTY(EditAnywhere)
	FText Text;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> ButtonText;

};
