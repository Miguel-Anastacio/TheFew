// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ArenaGameState.h"
#include "PlanePawnAI.h"
#include "UI/ScoreboardWidget.h"
void AArenaGameState::InitTeamData(const FTeam& teamA, const FTeam& teamB)
{
	TeamAData.ID = teamA.ID;
	TeamAData.TeamName = teamA.TeamName;
	for (auto& it : teamA.AIActors)
	{
		TeamAData.PlayersGameData.Add(it->GetGameName(), FPlayerGameData());
	}

	TeamBData.ID = teamB.ID;
	TeamBData.TeamName = teamB.TeamName;
	for (auto& it : teamB.AIActors)
	{
		TeamBData.PlayersGameData.Add(it->GetGameName(), FPlayerGameData());
	}
}

void AArenaGameState::UpdateTeamGameData(const FTeamGameData& data)
{
	if (data.ID == TeamAData.ID)
	{
		TeamAData = data;
	}
	else if (data.ID == TeamBData.ID)
	{
		TeamBData = data;
	}
}

FPlayerGameData AArenaGameState::IncreaseKills(const FString& playerName)
{
	FPlayerGameData* playerData = TeamAData.PlayersGameData.Find(playerName);

	if (playerData)
	{
		playerData->Kills++;
		TeamAData.Kills++;

		return *playerData;
	}

	playerData = TeamBData.PlayersGameData.Find(playerName);
	if (playerData)
	{
		playerData->Kills++;
		TeamBData.Kills++;

		return *playerData;
	}

	return FPlayerGameData();
}

FPlayerGameData AArenaGameState::IncreaseDeaths(const FString& playerName)
{
	FPlayerGameData* playerData = TeamAData.PlayersGameData.Find(playerName);

	if (playerData)
	{
		playerData->Deaths++;
		TeamAData.Deaths++;
		return *playerData;
	}

	playerData = TeamBData.PlayersGameData.Find(playerName);
	if (playerData)
	{
		playerData->Deaths++;
		TeamBData.Deaths++;
		return *playerData;
	}

	return FPlayerGameData();
}

void AArenaGameState::UpdateScoreboard(const FString& killer, const FString& victim)
{
	FPlayerGameData killerData = IncreaseKills(killer);
	FPlayerGameData victimData  = IncreaseDeaths(victim);
	if (IsValid(ScoreboardWidget))
	{
		int killerID = GetPlayerTeamID(killer);
		int victimID = GetPlayerTeamID(victim);
		ScoreboardWidget->UpdateScoreboard(killer, killerData, GetPlayerTeamID(killer));
		ScoreboardWidget->UpdateScoreboard(victim, victimData, GetPlayerTeamID(victim));
		
		// this is ass
		// each player needs to know about its team ID
		if (killerID == TeamAData.ID && victimID == TeamBData.ID)
		{
			ScoreboardWidget->UpdateScoreboardTotal(TeamAData.Kills, killerID);
			ScoreboardWidget->UpdateScoreboardTotal(TeamBData.Kills, victimID);
		}
		else if (killerID == TeamBData.ID && victimID == TeamAData.ID)
		{
			ScoreboardWidget->UpdateScoreboardTotal(TeamBData.Kills, killerID);
			ScoreboardWidget->UpdateScoreboardTotal(TeamAData.Kills, victimID);
		}

	}
}


FPlayerGameData AArenaGameState::GetPlayerGameData(const FString& playerName, int& out_teamID)
{
	FPlayerGameData* playerData = TeamAData.PlayersGameData.Find(playerName);

	if (playerData)
	{
		out_teamID = TeamAData.ID;
		return *playerData;
	}

	playerData = TeamBData.PlayersGameData.Find(playerName);
	if (playerData)
	{
		out_teamID = TeamBData.ID;
		return *playerData;
	}

	out_teamID = -1;
	return FPlayerGameData();
}

int AArenaGameState::GetPlayerTeamID(const FString& playerName)
{
	FPlayerGameData* playerData = TeamAData.PlayersGameData.Find(playerName);

	if (playerData)
	{
		return TeamAData.ID;
	}

	playerData = TeamBData.PlayersGameData.Find(playerName);
	if (playerData)
	{
		return TeamBData.ID;
	}

	return -1;
}

void AArenaGameState::SetScoreboardWidgetRef(UScoreboardWidget* widget)
{
	ScoreboardWidget = widget;
}