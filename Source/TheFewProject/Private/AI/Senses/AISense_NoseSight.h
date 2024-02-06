// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISense.h"
#include "AISense_NoseSight.generated.h"

USTRUCT(BlueprintType)
struct FAINoseSightEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sense")
	TObjectPtr<AActor> Instigator;
		// location of sight stimulus
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sense")
	FVector SightLocation;

	FAINoseSightEvent(AActor* instigator, const FVector& sightLocation) :
		Instigator(instigator), SightLocation(sightLocation)
	{
	};

	FAINoseSightEvent() {};
};

/**
 * 
 */
UCLASS()
class UAISense_NoseSight : public UAISense
{
	GENERATED_BODY()

public:

	struct FDigestedNoseSightProperties
	{
		float DetectionLength;
		float StartCylinderOffset;
		bool bDisplayDebugCylinder;

		FDigestedNoseSightProperties();
		FDigestedNoseSightProperties(class UAISenseConfig_NoseSight& senseConfig);
	};
	/* Consumed properties from config */
	TArray<FDigestedNoseSightProperties> DigestedProperties;

	void RegisterEvent(const FAINoseSightEvent& event);

protected:
	UPROPERTY()
	TArray<FAINoseSightEvent> NoseSightEvents;

	virtual float Update() override;

private:
	void PerformShapeSweep(const AActor* listenerActor, const FDigestedNoseSightProperties& sweepProperties, TArray<FHitResult>& out_Hits);
};
