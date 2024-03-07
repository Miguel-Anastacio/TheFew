// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUISelectionInputSignature);
/**
 * 
 */
UCLASS()
class UButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	class UButton* const GetButton() { return Button; };
	virtual void NativePreConstruct() override;

	UPROPERTY(EditAnywhere)
	FText Text;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnUISelectionInputSignature UIInputActionDelegate;

protected:
	//UFUNCTION()
	//	void OnInputActionUI();
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> ButtonText;

};
