// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SpawnMenuWidget.h"
#include "UI/TotalScoreWidget.h"
#include "Components/RichTextBlock.h"


void USpawnMenuWidget::UpdateScoreboardTotal(int32 score, int32 teamID)
{
	if (TotalScoreWidget)
		TotalScoreWidget->UpdateTotalScore(score, teamID);
}

void USpawnMenuWidget::InitTeamAData(const FString& teamName, int32 score, int32 teamID)
{
	if (TotalScoreWidget)
		TotalScoreWidget->InitTeamA(teamName, score, teamID);
}

void USpawnMenuWidget::InitTeamBData(const FString& teamName, int32 score, int32 teamID)
{
	if (TotalScoreWidget)
		TotalScoreWidget->InitTeamB(teamName, score, teamID);
}
