// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISenseConfig.h"
#include "AI/Senses/AISense_NoseSight.h"
#include "AISenseConfig_NoseSight.generated.h"

/**
 * 
 */

UCLASS(meta =(DisplayName = "AI NoseSight config"))
class UAISenseConfig_NoseSight : public UAISenseConfig
{
	GENERATED_BODY()

public:
	// Set common config values
	UAISenseConfig_NoseSight(const FObjectInitializer& ObjectInitializer);
	// Implementation Class that defines the behavior of the sense
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", NoClear, config)
		TSubclassOf<UAISense_NoseSight> Implementation;
	// Size of cylinder in front of plane nose
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense")
		float DetectionLength = 100.f;
	// Offset of cylinder in front of plane nose
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense")
		float StartCylinderOffset = 200.f;
	/* True if you want to display the debug sphere around the pawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config)
		bool bDisplayDebugCylinder = true;
	// return implementation class
	virtual TSubclassOf<UAISense> GetSenseImplementation() const override;
};
