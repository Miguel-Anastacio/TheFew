// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ScoreboardWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/PlayerScoreWidget.h"
#include "UI/TotalScoreWidget.h"
void UScoreboardWidget::NativePreConstruct()
{
	/*Super::NativePreConstruct();
	US*/
}

void UScoreboardWidget::InitTeamA(const FTeamGameData& team)
{
	teamAID = team.ID;
	if (TotalScoreWidget)
	{
		TotalScoreWidget->InitTeamA(team.TeamName, team.Kills, team.ID);
	}

	if (TeamABox)
	{
		for (auto& it : team.PlayersGameData)
		{
			UPlayerScoreWidget* widget = CreateWidget<UPlayerScoreWidget>(GetOwningPlayer(), PlayerScoreWidgetClass);
			widget->Init(it.Key, it.Value.Kills, it.Value.Deaths, it.Value.Score);
			TeamABox->AddChildToVerticalBox(widget);
			TeamAPlayerScores.Add(it.Key, widget);
		}
	}

}

void UScoreboardWidget::InitTeamB(const FTeamGameData& team)
{
	teamBID = team.ID;
	if (TotalScoreWidget)
	{
		TotalScoreWidget->InitTeamB(team.TeamName, team.Kills, team.ID);
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
	if(TotalScoreWidget)
		TotalScoreWidget->UpdateTotalScore(score, teamID);
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
