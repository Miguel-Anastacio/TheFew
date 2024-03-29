// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameDataUtils.generated.h"

USTRUCT(BlueprintType)
struct FPlayerGameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere)
		int Kills = 0;
	UPROPERTY(VisibleAnywhere)
		int Score = 0;
	UPROPERTY(VisibleAnywhere)
		int Deaths = 0;

	FPlayerGameData() {};
};

USTRUCT(BlueprintType)
struct FTeamGameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere)
		FString TeamName = FString();
	UPROPERTY(VisibleAnywhere)
		int Kills = 0;
	UPROPERTY(VisibleAnywhere)
		int Score = 0;
	UPROPERTY(VisibleAnywhere)
		int Deaths = 0;
	UPROPERTY(VisibleAnywhere)
		int ID = 0;

	UPROPERTY(VisibleAnywhere)
	TMap<FString, FPlayerGameData> PlayersGameData;

	FTeamGameData() {};
};

USTRUCT()
struct FSpawnData
{
	GENERATED_BODY()

	const TArray<TObjectPtr<class APlanePawn>>* EnemyTeamActors;
	UPROPERTY(VisibleAnywhere)
	FString Name;
	FRotator Rot;

	FSpawnData() {};
	FSpawnData(const TArray<TObjectPtr<class APlanePawn>>* enemies, const FString& name, const FRotator& rot = FRotator())
		: EnemyTeamActors(enemies), Name(name), Rot(rot)
	{

	};
};

// Disable copy constructor cause Tqueue does not allow it

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
	UPROPERTY(EditAnywhere, Category = "AI")
		FString AIGameName;
	UPROPERTY(EditAnywhere, Category = "Spawn")
		TObjectPtr<AActor> SpawnArea;

	UPROPERTY(EditAnywhere, Category = "Spawn")
		float SpawnHeight;

	UPROPERTY(EditAnywhere, Category = "Spawn")
		float SpawnCooldown;

	UPROPERTY(EditAnywhere, Category = "ID")
		int ID = 0;
	UPROPERTY(EditAnywhere, Category = "")
		FString TeamName = FString();

	//UPROPERTY(EditAnywhere, Category = "Collision")
	//TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;
	UPROPERTY(VisibleAnywhere, Category = "")
	TArray<TObjectPtr<class APlanePawn>> AIActors;

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
	UPROPERTY(VisibleAnywhere)
    TArray<FSpawnData> SpawnQueue;
};



