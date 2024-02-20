// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlanePawn.h"
#include "PlanePawnPlayer.generated.h"

/**
 * 
 */
UCLASS()
class APlanePawnPlayer : public APlanePawn
{
	GENERATED_BODY()
	
public:
	void PostInitializeComponents() override;
	void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void RespawnPlayer(const FVector& spawnLocation);
	void RespawnPlayer();
protected:
	void PlaneDeath(AActor* instigator) override;

	UPROPERTY(EditAnywhere)
		FVector InitialVelocity = FVector(1000, 0, 600);

	FRotator InitalRotation;
	FVector InitalLocation;

};
