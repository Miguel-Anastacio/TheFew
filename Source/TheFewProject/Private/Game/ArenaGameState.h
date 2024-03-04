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

	AArenaGameState();

	void InitTeamData(const FTeam& teamA, const FTeam& teamB);
	void InitTeamID(const FTeam& teamA, const FTeam& teamB);
	void UpdateTeamGameData(const FTeamGameData& data);
	void AddPlayerToTeam(const FString& playerName, int32 id);
	
	void UpdateScoreboard(const FString& killer, const FString& victim);
	FPlayerGameData GetPlayerGameData(const FString& playerName, int& out_teamID);
	int GetPlayerTeamID(const FString& playerName);

	FORCEINLINE FTeamGameData GetTeamAData() { return TeamAData; };
	FORCEINLINE FTeamGameData GetTeamBData() { return TeamBData; };

	void SetScoreboardWidgetRef(class UScoreboardWidget* widget);
	void SetScoreboardSpawnWidgetRef(class USpawnMenuWidget* widget);
	void SetHUDScoreWidgetRef(class UTotalScoreWidget* widget);

	float GameTimer = 0.0f;
protected:
	void PostInitializeComponents() override;
	void BeginPlay() override;
	void Tick(float dt) override;

	FPlayerGameData IncreaseKills(const FString& playerName);
	FPlayerGameData IncreaseDeaths(const FString& playerName);

	// Max Game Time in Seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game End")
		float MaxTime = 600.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game End")
		int32 MaxKills = 25;

	bool GameActive = false;

private:
	UPROPERTY(VisibleAnywhere)
	FTeamGameData TeamAData;
	UPROPERTY(VisibleAnywhere)
	FTeamGameData TeamBData;

	TWeakObjectPtr<class UScoreboardWidget> ScoreboardWidget;
	TWeakObjectPtr <class USpawnMenuWidget> SpawMenuWidget;
	TWeakObjectPtr <class UTotalScoreWidget> HudScoreWidget;



};
