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

protected:
	void BeginPlay() override;
};
