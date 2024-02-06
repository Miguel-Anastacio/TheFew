// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Senses/AISense_NoseSight.h"
#include "AISenseConfig_NoseSight.h"
void UAISense_NoseSight::RegisterEvent(const FAINoseSightEvent& event)
{
	NoseSightEvents.Add(event);
	RequestImmediateUpdate();
}

float UAISense_NoseSight::Update()
{
	AIPerception::FListenerMap& ListenersMap = *GetListeners();

	// // Iterate through all the listeners that have the noseSight sense. Listeners are actors with an AIPerception component.
	for (AIPerception::FListenerMap::TIterator listenerIt(ListenersMap); listenerIt; ++listenerIt)
	{
		FPerceptionListener& listener = listenerIt->Value;
		const AActor* listenerBodyActor = listener.GetBodyActor();
		// skip listeners not interested in this sense
		if (!listener.HasSense(GetSenseID()))
		{
			continue;
		}

		//for (const auto& event : NoseSightEvents)
		//{
		//	/*
		//	* Let the listener know that it has received a stimulus from this event
		//	* The relevant info is provided
		//	*/


		//	listener.RegisterStimulus(event.Instigator, FAIStimulus(*this, 1.0f, event.SightLocation, listener.CachedLocation));
		//}


		for (auto& properties : DigestedProperties)
		{
			TArray<FHitResult> hitsResults;
			PerformShapeSweep(listenerBodyActor, properties, hitsResults);

			for (auto& hit : hitsResults)
			{
				if (hit.GetActor()->ActorHasTag(FName("Terrain")))
				{
					listener.RegisterStimulus(hit.GetActor(), FAIStimulus(*this, 1.0f, hit.Location, listenerBodyActor->GetActorLocation()));
				}
			}
		}

	}
	// remove all currently registered nosesight events, so they only get handled once
	NoseSightEvents.Reset();

	// next tick of this function won't happen on it's own only if RequestImmediateUpdate is called again
	return SuspendNextUpdate;
}

void UAISense_NoseSight::PerformShapeSweep(const AActor* listenerActor, const FDigestedNoseSightProperties& sweepProperties, TArray<FHitResult>& out_Hits)
{
	// create cylinder
	FCollisionShape collisionBox = FCollisionShape::MakeBox(FVector(sweepProperties.DetectionLength, 200.0f, 300.0f));
	FVector start = listenerActor->GetActorLocation() + listenerActor->GetActorForwardVector() * sweepProperties.StartCylinderOffset;
	FVector end = start + collisionBox.GetBox().X * listenerActor->GetActorForwardVector();
	GetWorld()->SweepMultiByChannel(out_Hits, start, end, FQuat(), ECollisionChannel::ECC_Visibility, collisionBox);

	if (sweepProperties.bDisplayDebugCylinder)
	{
		DrawDebugBox(GetWorld(), start, collisionBox.GetExtent(), FColor::Blue, false, 2.0f, 1, 2.0f);
	}

}

UAISense_NoseSight::FDigestedNoseSightProperties::FDigestedNoseSightProperties()
{
	// init
	DetectionLength = 400.0f;
	StartCylinderOffset = 150.0f;
	bDisplayDebugCylinder = false;
}

UAISense_NoseSight::FDigestedNoseSightProperties::FDigestedNoseSightProperties(UAISenseConfig_NoseSight& senseConfig)
{
	//Copy constructor
	DetectionLength = senseConfig.DetectionLength;
	bDisplayDebugCylinder = senseConfig.bDisplayDebugCylinder;
	StartCylinderOffset = senseConfig.StartCylinderOffset;
}
