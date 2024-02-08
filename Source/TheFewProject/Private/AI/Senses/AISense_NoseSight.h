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
		FDigestedNoseSightProperties(const class UAISenseConfig_NoseSight& senseConfig);
	};
	/* Consumed properties from config */
	TArray<FDigestedNoseSightProperties> DigestedProperties;

	UAISense_NoseSight();
	void RegisterEvent(const FAINoseSightEvent& event);

protected:
	UPROPERTY()
	TArray<FAINoseSightEvent> NoseSightEvents;

	virtual float Update() override;

	/* A listener is someone who has a Perception component with various senses
	 * This function will be called when a new listener gained this sense
	 */
	void OnNewListenerImpl(const FPerceptionListener& NewListener);

	/*
	 * Called whenever the listener is removed (eg destroyed or game has stopped)
	 */
	void OnListenerRemovedImpl(const FPerceptionListener& UpdatedListener);

private:
	void PerformShapeSweep(const AActor* listenerActor, const FDigestedNoseSightProperties& sweepProperties, TArray<FHitResult>& out_Hits);
};
