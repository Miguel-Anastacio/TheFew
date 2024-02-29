// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TotalScoreWidget.h"
#include "UI/StatisticWidget.h"
#include "Components/RichTextBlock.h"

void UTotalScoreWidget::InitTeamA(const FString& teamName, int32 score, int32 teamID)
{
	teamAID = teamID;
	/*if (TeamAName)
	{
		TeamAName->SetText(FText::FromString(teamName));
	}
	if (TeamAScore)
	{
		TeamAScore->SetText(FText::AsNumber(score));
	}*/
	if (TeamAWidget)
	{
		TeamAWidget->Init(teamName, score);
	}
}

void UTotalScoreWidget::InitTeamB(const FString& teamName, int32 score, int32 teamID)
{
	teamBID = teamID;
	if (TeamBWidget)
	{
		TeamBWidget->Init(teamName, score);
	}
}

void UTotalScoreWidget::UpdateTotalScore(int32 score, int32 teamID)
{
	if (teamID == teamAID)
	{
		TeamAWidget->UpdateValue(score);
	}
	else if (teamID == teamBID)
	{
		TeamBWidget->UpdateValue(score);
	}
}
