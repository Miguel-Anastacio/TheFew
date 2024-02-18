// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/GameDataUtils.h"
#include "AIManager.generated.h"

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
	void SpawnAIActor(FTeam& team, const FString& name = FString(), const FRotator& rot = FRotator(0, 0, 0));
	UFUNCTION()
	void SpawnAIActorGameInProgress(FTeam& team, const TArray<class APlanePawnAI*>& enemyTeamActors, const FString& name = FString(), const FRotator& rot = FRotator(0, 0, 0));
	void Respawn(FTeam& team, const TArray<TObjectPtr<class APlanePawnAI>>& enemyTeamActors, const FString& name = FString(), const FRotator& rot = FRotator(0, 0, 0));
	void UpdateTarget(AActor* actor);
	TObjectPtr<class APlanePawnAI> ChangePlaneSelected(float input);


	void IncreaseCrashes() { Crashes++; };
	void IncreaseTargetsDestroyed() { TargetsDestroyed++; };

	void IncreaseTeamScore(int teamID);
	
protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
		void OnAIDestroyed(AActor* actor);

	void SpawnTeam(FTeam& team, const FRotator& rot = FRotator(0, 0, 0));
	void InitTeamTargets(FTeam& team1, FTeam& team2);
	
	void UpdateTarget(AActor* currentTarget, FTeam& teamToUpdate, const TArray<TObjectPtr<class APlanePawnAI>>& newTargets);

	void RemoveElementFromTeam(FTeam& team, const FString& elementName);
protected:
	////landscape actor
	UPROPERTY(EditAnywhere, Category = "Level")
		TObjectPtr<AActor> LevelLandscape;

	UPROPERTY(EditAnywhere)
		FTeam TeamA;
	UPROPERTY(EditAnywhere)
		FTeam TeamB;

	UPROPERTY(EditAnywhere, Category = "Level")
		TArray<TObjectPtr<AActor>> Targets;

	UPROPERTY(EditAnywhere, Category = "Level")
		TObjectPtr<AActor> PatrolDestination;

	int PlaneSelectedIndex = -1;


	UPROPERTY(VisibleAnywhere, Category = "Data")
	int Crashes = 0;
	UPROPERTY(VisibleAnywhere, Category = "Data")
	int TargetsDestroyed = 0;


	int CurrentTeamID = 0;
	//UPROPERTY

	friend class APlanePhysicsDebugHUD;
};
