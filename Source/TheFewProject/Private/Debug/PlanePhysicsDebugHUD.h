// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Debug/DebugHUD.h"
#include "PlanePhysicsDebugHUD.generated.h"

/**
 * 
 */
class APlanePawn;
UCLASS()
class APlanePhysicsDebugHUD : public ADebugHUD
{
	GENERATED_BODY()

protected:
	// Draw the HUD.
	virtual void DrawHUD() override;
	void DrawSteering(APlanePawn* pl);
	void DrawThrust(APlanePawn* pl);
	void DrawPlaneData(APlanePawn* pl);
};
