// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlanePawn.h"
#include "PlanePawnAI.generated.h"

/**
 * 
 */
UCLASS()
class APlanePawnAI : public APlanePawn
{
	GENERATED_BODY()
public:
	APlanePawnAI();
	TObjectPtr<class UBoxComponent> GetDetectionVolume();
	void Tick(float deltaTime) override;
protected:
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Obstacle Detection")
		TObjectPtr<class UBoxComponent> DetectionVolume;

};
