// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ArenaGameState.h"
#include "PlanePawnAI.h"
#include "UI/ScoreboardWidget.h"
#include "UI/SpawnMenuWidget.h"
#include "UI/TotalScoreWidget.h"
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

void AArenaGameState::InitTeamID(const FTeam& teamA, const FTeam& teamB)
{
	TeamAData.ID = teamA.ID;
	TeamAData.TeamName = teamA.TeamName;
	TeamBData.ID = teamB.ID;
	TeamBData.TeamName = teamB.TeamName;
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

void AArenaGameState::AddPlayerToTeam(const FString& playerName, int32 id)
{
	if (id == TeamAData.ID)
	{
		if (TeamAData.PlayersGameData.Find(playerName) == NULL)
		{
			TeamAData.PlayersGameData.Add(playerName, FPlayerGameData());
		}
	}
	else if (id == TeamBData.ID)
	{
		if (TeamBData.PlayersGameData.Find(playerName) == NULL)
		{
			TeamBData.PlayersGameData.Add(playerName, FPlayerGameData());
		}
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
	int killerID = GetPlayerTeamID(killer);
	int victimID = GetPlayerTeamID(victim);

	if (IsValid(ScoreboardWidget.Get()))
	{
		ScoreboardWidget->UpdateScoreboard(killer, killerData, GetPlayerTeamID(killer));
		ScoreboardWidget->UpdateScoreboard(victim, victimData, GetPlayerTeamID(victim));


		if (killerID == TeamAData.ID && victimID == TeamBData.ID)
		{
			ScoreboardWidget->UpdateScoreboardTotal(TeamAData.Kills, killerID);
			//ScoreboardWidget->UpdateScoreboardTotal(TeamBData.Kills, victimID);
		}
		else if (killerID == TeamBData.ID && victimID == TeamAData.ID)
		{
			ScoreboardWidget->UpdateScoreboardTotal(TeamBData.Kills, killerID);
			//ScoreboardWidget->UpdateScoreboardTotal(TeamAData.Kills, victimID);
		}

	}

	if (IsValid(SpawMenuWidget.Get()))
	{
		if (killerID == TeamAData.ID && victimID == TeamBData.ID)
		{
			SpawMenuWidget->UpdateScoreboardTotal(TeamAData.Kills, killerID);
		}
		else if (killerID == TeamBData.ID && victimID == TeamAData.ID)
		{
			SpawMenuWidget->UpdateScoreboardTotal(TeamBData.Kills, killerID);
		}
	}

	if (IsValid(HudScoreWidget.Get()))
	{
		if (killerID == TeamAData.ID && victimID == TeamBData.ID)
		{
			HudScoreWidget->UpdateTotalScore(TeamAData.Kills, killerID);
		}
		else if (killerID == TeamBData.ID && victimID == TeamAData.ID)
		{
			HudScoreWidget->UpdateTotalScore(TeamBData.Kills, killerID);
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

void AArenaGameState::SetScoreboardSpawnWidgetRef(USpawnMenuWidget* widget)
{
	SpawMenuWidget = widget;
}

void AArenaGameState::SetHUDScoreWidgetRef(UTotalScoreWidget* widget)
{
	widget->InitTeamA(TeamAData.TeamName, TeamAData.Kills, TeamAData.ID);
	widget->InitTeamB(TeamBData.TeamName, TeamBData.Kills, TeamBData.ID);
	HudScoreWidget = widget;

}
