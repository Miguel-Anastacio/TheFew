// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/GameDataUtils.h"
#include "ScoreboardWidget.generated.h"

/**
 * 
 */
UCLASS()
class UScoreboardWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void InitTeamA(const FTeamGameData& team);
	void InitTeamB(const FTeamGameData& team);

	void UpdateScoreboard(const FString& playerName, const FPlayerGameData& data, int32 teamID);
	void UpdateScoreboardTotal(int32 score, int32 teamID);

protected:

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class URichTextBlock> TeamAName;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class URichTextBlock> TeamAScore;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class URichTextBlock> TeamBName;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class URichTextBlock> TeamBScore;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UVerticalBox> TeamABox;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UVerticalBox> TeamBBox;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UPlayerScoreWidget> PlayerScoreWidgetClass;

	TMap<FString, TObjectPtr<class UPlayerScoreWidget>> TeamAPlayerScores;

	TMap<FString, TObjectPtr<class UPlayerScoreWidget>> TeamBPlayerScores;


	int teamAID = 0;
	int teamBID = 0;


};
