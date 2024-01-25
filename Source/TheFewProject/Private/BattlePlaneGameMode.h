// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattlePlaneGameMode.generated.h"

/**
 * 
 */
class APlanePawn;
UCLASS()
class ABattlePlaneGameMode : public AGameModeBase
{
	GENERATED_BODY()

	ABattlePlaneGameMode();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Pawn")
		TSubclassOf<class APlanePawn> PlanePawnClass;

	
};
