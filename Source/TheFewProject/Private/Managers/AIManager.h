// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/GameDataUtils.h"
#include "AIManager.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeamDataInitSignature);

UCLASS()
class AAIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//void SpawnAIActor(const FVector2D& minBounds, const FVector2D& maxBounds, float zHeight);
	void SpawnAIActor(FTeam& team, const FString& name = FString(), const FRotator& rot = FRotator(0, 0, 0), const FVector& location = FVector(0, 0, 0));
	UFUNCTION()
	void SpawnAIActorGameInProgress(FTeam& team, const TArray<class APlanePawn*>& enemyTeamActors, const FString& name = FString(), const FRotator& rot = FRotator(0, 0, 0));

	void Respawn(FTeam& team, const TArray<TObjectPtr<class APlanePawn>>& enemyTeamActors, const FString& name = FString(), const FRotator& rot = FRotator(0, 0, 0));
	void UpdateTarget(AActor* actor);
	TObjectPtr<class APlanePawn> ChangePlaneSelected(float input);

	void IncreaseCrashes() { Crashes++; };
	void IncreaseTargetsDestroyed() { TargetsDestroyed++; };

	void IncreaseTeamScore(int teamID);

public:
	FOnTeamDataInitSignature TeamDataInitDelegate;
	
protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
		void OnAIDestroyed(AActor* actor);
	UFUNCTION()
		void OnAITargetShifted(AActor* actor);
	UFUNCTION()
		void OnPlayerSpawn(AActor* player);
	UFUNCTION()
		void OnPlayerDeath(AActor* player);


	UFUNCTION()
		void OnGameEnd();

	void SpawnTeam(FTeam& team, const FVector& offsetSpawn, const FRotator& rot = FRotator(0, 0, 0));
	void InitTeamTargets(FTeam& team1, FTeam& team2);
	
	void UpdateTarget(AActor* currentTarget, FTeam& teamToUpdate, const TArray<TObjectPtr<class APlanePawn>>& newTargets);

	void RemoveElementFromTeam(FTeam& team, const FString& elementName);

	void HandleSpawnQueue(FTeam& team);
	int TeamMembersTargetingActor(const FTeam& team, AActor* actor);
protected:
	////landscape actor
	UPROPERTY(EditAnywhere, Category = "Level")
		TObjectPtr<AActor> LevelLandscape;

	UPROPERTY(EditAnywhere, Category = "Team A")
		FTeam TeamA;
	UPROPERTY(EditAnywhere, Category = "Team A")
		FVector SpawnOffsetA;
	UPROPERTY(EditAnywhere, Category = "Team B")
		FTeam TeamB;
	UPROPERTY(EditAnywhere, Category = "Team B")
		FVector SpawnOffsetB;


	UPROPERTY(EditAnywhere, Category = "Level")
		TArray<TObjectPtr<AActor>> Targets;

	UPROPERTY(EditAnywhere, Category = "Level")
		TArray<TObjectPtr<AActor>> PatrolDestinations;

	UPROPERTY(EditAnywhere, Category = "AI Behaviour")
	int32 EnemiesTargetingPlayer = 3;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	int32 CurrentEnemiesTargetingPlayer = 0;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	int Crashes = 0;
	UPROPERTY(VisibleAnywhere, Category = "Data")
	int TargetsDestroyed = 0;

	int PlaneSelectedIndex = -1;
	int CurrentTeamID = 0;

	bool GameActive = true;
	friend class APlanePhysicsDebugHUD;
};
