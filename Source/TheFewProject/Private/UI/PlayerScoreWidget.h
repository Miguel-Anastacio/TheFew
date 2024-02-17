// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class UPlayerScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/*class UButton* GetButton() { return Button; };
	virtual void NativePreConstruct() override;

	UPROPERTY(EditAnywhere)
	FText Text;*/

	void Init(const FString& name, int32 kills, int32 deaths, int32 score);
	void Update(int32 kills, int32 deaths, int32 score);

protected:
	//UPROPERTY(meta = (BindWidget))
	//	TObjectPtr<class URichTextBlock> Rank;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> PlayerName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> Kills;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class URichTextBlock> Deaths;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class URichTextBlock> Score;

};
