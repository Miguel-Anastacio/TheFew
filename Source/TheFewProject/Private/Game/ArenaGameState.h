// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameDataUtils.h"
#include "ArenaGameState.generated.h"

UCLASS(Blueprintable)
class AArenaGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	void InitTeamData(const FTeam& teamA, const FTeam& teamB);
	void InitTeamID(const FTeam& teamA, const FTeam& teamB);
	void UpdateTeamGameData(const FTeamGameData& data);
	void AddPlayerToTeam(const FString& playerName, int32 id);
	
	FPlayerGameData IncreaseKills(const FString& playerName);
	FPlayerGameData IncreaseDeaths(const FString& playerName);
	void UpdateScoreboard(const FString& killer, const FString& victim);
	FPlayerGameData GetPlayerGameData(const FString& playerName, int& out_teamID);
	int GetPlayerTeamID(const FString& playerName);

	FORCEINLINE FTeamGameData GetTeamAData() { return TeamAData; };
	FORCEINLINE FTeamGameData GetTeamBData() { return TeamBData; };

	void SetScoreboardWidgetRef(class UScoreboardWidget* widget);
	void SetScoreboardSpawnWidgetRef(class USpawnMenuWidget* widget);
private:
	UPROPERTY(VisibleAnywhere)
	FTeamGameData TeamAData;
	UPROPERTY(VisibleAnywhere)
	FTeamGameData TeamBData;

	TObjectPtr<class UScoreboardWidget> ScoreboardWidget;
	TObjectPtr<class USpawnMenuWidget> SpawMenuWidget;

};
