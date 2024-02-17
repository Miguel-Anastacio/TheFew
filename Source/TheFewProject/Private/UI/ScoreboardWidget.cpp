// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ScoreboardWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/PlayerScoreWidget.h"

void UScoreboardWidget::InitTeamA(const FTeamGameData& team)
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
	if (TeamABox)
	{
		for (auto& it : team.PlayersGameData)
		{
			UPlayerScoreWidget* widget = CreateWidget<UPlayerScoreWidget>(GetOwningPlayer(), PlayerScoreWidgetClass);
			widget->Init(it.Key, it.Value.Kills, it.Value.Deaths, it.Value.Score);
			TeamABox->AddChildToVerticalBox(widget);
			TeamAPlayerScores.Add(it.Key, widget);
			//
		}
	}

}

void UScoreboardWidget::InitTeamB(const FTeamGameData& team)
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

	if (TeamBBox)
	{
		for (auto& it : team.PlayersGameData)
		{
			UPlayerScoreWidget* widget = CreateWidget<UPlayerScoreWidget>(GetOwningPlayer(), PlayerScoreWidgetClass);
			widget->Init(it.Key, it.Value.Kills, it.Value.Deaths, it.Value.Score);
			TeamBBox->AddChildToVerticalBox(widget);
			TeamBPlayerScores.Add(it.Key, widget);
		}
	}
}

void UScoreboardWidget::UpdateScoreboardTotal(int32 score, int32 teamID)
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

void UScoreboardWidget::UpdateScoreboard(const FString& playerName, const FPlayerGameData& data, int32 teamID)
{
	if (teamID == teamAID)
	{
		TObjectPtr<UPlayerScoreWidget>* widget = TeamAPlayerScores.Find(playerName);
		if (widget)
		{
			widget->Get()->Update(data.Kills, data.Deaths, data.Score);
		}
	}
	else if (teamID == teamBID)
	{
		TObjectPtr<UPlayerScoreWidget>* widget = TeamBPlayerScores.Find(playerName);
		if (widget)
		{
			widget->Get()->Update(data.Kills, data.Deaths, data.Score);
		}
	}
}
