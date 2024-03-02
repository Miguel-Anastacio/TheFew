// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ScoreboardWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/ScrollBox.h"
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
		int maxLength = 25;
		for (auto& it : team.PlayersGameData)
		{
			if (it.Key.Len() > maxLength)
				maxLength = it.Key.Len();
		}

		for (auto& it : team.PlayersGameData)
		{
			UPlayerScoreWidget* widget = CreateWidget<UPlayerScoreWidget>(GetOwningPlayer(), PlayerScoreWidgetClass);
			const int dif = maxLength - it.Key.Len();
			FString name = it.Key;
			for(int i = 0; i < dif; i++)
			{
				name += " ";
			}

			widget->Init(name, it.Value.Kills, it.Value.Deaths, it.Value.Score);
			TeamABox->AddChild(widget);
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
		int maxLength = 0;
		for (auto& it : team.PlayersGameData)
		{
			if (it.Key.Len() > maxLength)
				maxLength = it.Key.Len();
		}
		for (auto& it : team.PlayersGameData)
		{
			UPlayerScoreWidget* widget = CreateWidget<UPlayerScoreWidget>(GetOwningPlayer(), PlayerScoreWidgetClass);
			const int dif = maxLength - it.Key.Len();
			FString name = it.Key;
			for (int i = 0; i < dif; i++)
			{
				name += " ";
			}
			widget->Init(name, it.Value.Kills, it.Value.Deaths, it.Value.Score);
			TeamBBox->AddChild(widget);
			TeamBPlayerScores.Add(it.Key, widget);
		}
	}
}

void UScoreboardWidget::UpdateScoreboardTotal(int32 score, int32 teamID)
{
	if(TotalScoreWidget)
		TotalScoreWidget->UpdateTotalScore(score, teamID);
}

void UScoreboardWidget::SetBoxSize()
{
	/*if (TeamABox)
	{
		Te
	}*/
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
