// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIManager.generated.h"
USTRUCT(BlueprintType)
struct FTeam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "AI")
		FLinearColor AIWidgetColor = FLinearColor::Blue;

	UPROPERTY(EditAnywhere, Category = "AI")
		TSubclassOf<class APlanePawnAI> AIClass;

	UPROPERTY(EditAnywhere, Category = "AI")
		int NumberOfAIActors = 20.f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
		TObjectPtr<AActor> SpawnArea;

	UPROPERTY(EditAnywhere, Category = "Spawn")
		float SpawnHeight;

	UPROPERTY(EditAnywhere, Category = "Spawn")
		float SpawnCooldown;

	UPROPERTY(EditAnywhere, Category = "Score")
		float Score = 0.0f;
	UPROPERTY(EditAnywhere, Category = "ID")
		int ID = 0;

	TArray<TObjectPtr<class APlanePawnAI>> AIActors;

	FVector2D SpawnAreaBoundsMax;
	FVector2D SpawnAreaBoundsMin;

	float TimeSinceLastSpawn = 0.0f;

	FTimerHandle SpawnTimerHandler;

	void InitSpawnAreaBounds()
	{
		if (SpawnArea)
		{
			FVector origin, extent;
			SpawnArea->GetActorBounds(false, origin, extent);
			SpawnAreaBoundsMax.X = origin.X + extent.X;
			SpawnAreaBoundsMax.Y = origin.Y + extent.Y;

			SpawnAreaBoundsMin.X = origin.X - extent.X;
			SpawnAreaBoundsMin.Y = origin.Y - extent.Y;
		}
	}
};


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
	void SpawnAIActor(FTeam& team, const FRotator& rot = FRotator(0, 0, 0));
	UFUNCTION()
	void SpawnAIActorGameInProgress(FTeam& team, const TArray<class APlanePawnAI*>& enemyTeamActors, const FRotator& rot = FRotator(0, 0, 0));
	void Respawn(FTeam& team, const TArray<TObjectPtr<class APlanePawnAI>>& enemyTeamActors, const FRotator& rot = FRotator(0, 0, 0));
	void UpdateTarget(AActor* actor);
	TObjectPtr<class APlanePawnAI> ChangePlaneSelected(float input);


	void IncreaseCrashes() { Crashes++; };
	void IncreaseTargetsDestroyed() { TargetsDestroyed++; };
	
protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
		void OnAIDestroyed(AActor* actor);

	void SpawnTeam(FTeam& team, const FRotator& rot = FRotator(0, 0, 0));

protected:
	//UPROPERTY(EditAnywhere, Category = "AI")
	//	TSubclassOf<class APlanePawnAI> AIClass;
	//
	//// TeamA
	//UPROPERTY(EditAnywhere, Category = "AI")
	//	FColor AIWidgetTeamA = FColor::Blue;
	//UPROPERTY(EditAnywhere, Category = "AI")
	//	FColor AIWidgetColorTeamB = FColor::Red;

	//TArray<TObjectPtr<class APlanePawnAI>> AIActorsTeamB;
	//UPROPERTY(EditAnywhere, Category = "Amounts")
	//	int NumberOfAITeamA = 20.f;
	//UPROPERTY(EditAnywhere, Category = "Amounts")
	//	int NumberOfAITeamB = 20.f;
	////landscape actor
	UPROPERTY(EditAnywhere, Category = "Level")
		TObjectPtr<AActor> LevelLandscape;


	UPROPERTY(EditAnywhere)
		FTeam TeamA;
	UPROPERTY(EditAnywhere)
		FTeam TeamB;

	UPROPERTY(EditAnywhere, Category = "Level")
		TArray<TObjectPtr<AActor>> Targets;

	int PlaneSelectedIndex = -1;


	UPROPERTY(VisibleAnywhere, Category = "Data")
	int Crashes = 0;
	UPROPERTY(VisibleAnywhere, Category = "Data")
	int TargetsDestroyed = 0;


	int CurrentTeamID = 0;
	//UPROPERTY

};
