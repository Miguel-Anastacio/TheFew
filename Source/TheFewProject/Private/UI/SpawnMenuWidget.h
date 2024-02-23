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
	void UpdateScoreboardTotal(int32 score, int32 teamID);
	void InitTeamAData(const FString& teamName, int32 score, int32 teamID);
	void InitTeamBData(const FString& teamName, int32 score, int32 teamID);

protected:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTotalScoreWidget> TotalScoreWidget;

	int teamAID = 0;
	int teamBID = 0;
};
