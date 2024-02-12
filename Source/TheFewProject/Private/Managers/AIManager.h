// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	void SpawnEnemy(const FVector2D& minBounds, const FVector2D& maxBounds, float zHeight);
	void UpdateTarget(AActor* actor);
	TObjectPtr<class APlanePawnAI> ChangePlaneSelected(float input);


	void IncreaseCrashes() { Crashes++; };
	void IncreaseTargetsDestroyed() { TargetsDestroyed++; };
	
protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnAIDestroyed(AActor* actor);

	// debug function used to focus on an Ai plane

protected:
	UPROPERTY(EditAnywhere, Category = "Enemy")
		TSubclassOf<class APlanePawnAI> EnemyClass;
	
	TArray<TObjectPtr<class APlanePawnAI>> Enemies;



	UPROPERTY(EditAnywhere, Category = "Amounts")
		int NumberOfEnemies = 25.f;
	//landscape actor
	UPROPERTY(EditAnywhere, Category = "Level")
		TObjectPtr<AActor> SpawnArea;
	UPROPERTY(EditAnywhere, Category = "Level")
		TArray<TObjectPtr<AActor>> Targets;
	UPROPERTY(EditAnywhere, Category = "Level")
		float SpawnHeight;
	int PlaneSelectedIndex = -1;

	FVector2D SpawnAreaBoundsMax;
	FVector2D SpawnAreaBoundsMin;
	UPROPERTY(VisibleAnywhere, Category = "Data")
	int Crashes = 0;
	UPROPERTY(VisibleAnywhere, Category = "Data")
	int TargetsDestroyed = 0;

	//UPROPERTY

};
