// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TotalScoreWidget.h"
#include "Components/RichTextBlock.h"

void UTotalScoreWidget::InitTeamA(const FString& teamName, int32 score, int32 teamID)
{
	teamAID = teamID;
	if (TeamAName)
	{
		TeamAName->SetText(FText::FromString(teamName));
	}
	if (TeamAScore)
	{
		TeamAScore->SetText(FText::AsNumber(score));
	}

}

void UTotalScoreWidget::InitTeamB(const FString& teamName, int32 score, int32 teamID)
{
	teamBID = teamID;
	if (TeamBName)
	{
		TeamBName->SetText(FText::FromString(teamName));
	}
	if (TeamBScore)
	{
		TeamBScore->SetText(FText::AsNumber(score));
	}
}

void UTotalScoreWidget::UpdateTotalScore(int32 score, int32 teamID)
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
