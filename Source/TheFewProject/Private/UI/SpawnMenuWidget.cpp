// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SpawnMenuWidget.h"
#include "Components/RichTextBlock.h"
void USpawnMenuWidget::InitTeamA(const FTeamGameData& team)
{
	teamAID = team.ID;
	if (TeamAName)
	{
		TeamAName->SetText(FText::FromString(team.TeamName));
	}
	if (TeamAScore)
	{
		TeamAScore->SetText(FText::AsNumber(team.Kills));
	}
}

void USpawnMenuWidget::InitTeamB(const FTeamGameData& team)
{
	teamBID = team.ID;
	if (TeamBName)
	{
		TeamBName->SetText(FText::FromString(team.TeamName));
	}
	if (TeamBScore)
	{
		TeamBScore->SetText(FText::AsNumber(team.Kills));
	}
}

void USpawnMenuWidget::UpdateScoreboardTotal(int32 score, int32 teamID)
{
	if (teamID == teamAID)
	{
		TeamAScore->SetText(FText::AsNumber(score));
	}
	else if (teamID == teamBID)
	{
		TeamBScore->SetText(FText::AsNumber(score));
	}
}
