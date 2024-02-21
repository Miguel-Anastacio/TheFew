// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/GameDataUtils.h"
#include "SpawnMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class USpawnMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitTeamA(const FTeamGameData& team);
	void InitTeamB(const FTeamGameData& team);
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

	int teamAID = 0;
	int teamBID = 0;
};
