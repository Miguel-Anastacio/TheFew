// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TotalScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class UTotalScoreWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void InitTeamA(const FString& teamName, int32 score, int32 teamID);
	void InitTeamB(const FString& teamName, int32 score, int32 teamID);

	void UpdateTotalScore(int32 score, int32 teamID);

protected:

	/*UPROPERTY(meta = (BindWidget))
		TObjectPtr<class URichTextBlock> TeamAName;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class URichTextBlock> TeamAScore;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class URichTextBlock> TeamBName;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class URichTextBlock> TeamBScore;*/
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UStatisticWidget> TeamAWidget;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UStatisticWidget> TeamBWidget;
	int teamAID = 0;
	int teamBID = 0;


};
