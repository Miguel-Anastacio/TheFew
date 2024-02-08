// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Senses/AISense_NoseSight.h"
#include "AISenseConfig_NoseSight.h"
#include "Perception/AIPerceptionComponent.h"
#include "../TheFewProject.h"
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
			/*if (hitsResults)
			{
				listener.RegisterStimulus(NULL, FAIStimulus(*this, 1.0f, FVector(0, 0, 0), listenerBodyActor->GetActorLocation()));
			}*/

			for (auto& hit : hitsResults)
			{
				//listener.RegisterStimulus(hit.GetActor(), FAIStimulus(*this, 1.0f, hit.Location, listenerBodyActor->GetActorLocation()));
				/*if (hit.GetActor()->ActorHasTag(FName("Terrain")))
				{
				}*/
			}

		}

	}
	// remove all currently registered nosesight events, so they only get handled once
	//NoseSightEvents.Reset();

	return 0.f;
}

UAISense_NoseSight::UAISense_NoseSight()
{
	OnNewListenerDelegate.BindUObject(this, &UAISense_NoseSight::OnNewListenerImpl);
	OnListenerRemovedDelegate.BindUObject(this, &UAISense_NoseSight::OnListenerRemovedImpl);
}

void UAISense_NoseSight::OnNewListenerImpl(const FPerceptionListener& NewListener)
{
	//Since we have at least one AI actor with this sense this function will fire when the game starts
	UE_LOG(LogProjectFew, Warning, TEXT("Hello new Listener"));
	check(NewListener.Listener.IsValid());
	//Get the config
	UAISenseConfig* Config = NewListener.Listener->GetSenseConfig(GetSenseID());
	const UAISenseConfig_NoseSight* SenseConfig = Cast<const UAISenseConfig_NoseSight>(Config);
	check(SenseConfig);
	//Consume properties from the sense config
	FDigestedNoseSightProperties PropertyDigest(*SenseConfig);
	DigestedProperties.Add(PropertyDigest);
	RequestImmediateUpdate();
}

void UAISense_NoseSight::OnListenerRemovedImpl(const FPerceptionListener& UpdatedListener)
{
	//In our case, executes when we stop playing
	//GLog->Log("on listener removed!");
	UE_LOG(LogProjectFew, Warning, TEXT("On Listener Removed"));
}

void UAISense_NoseSight::PerformShapeSweep(const AActor* listenerActor, const FDigestedNoseSightProperties& sweepProperties, TArray<FHitResult>& out_Hits)
{
	// create cylinder
	FVector box = FVector(sweepProperties.DetectionLength, 400, 600.0f);
	FVector boxLocal = listenerActor->GetTransform().InverseTransformVector(box);
	boxLocal = boxLocal.GetAbs();
	boxLocal = box;

	FCollisionShape collisionBox = FCollisionShape::MakeBox(boxLocal);
	FVector start = listenerActor->GetActorLocation() + listenerActor->GetActorForwardVector() * sweepProperties.StartCylinderOffset;
	FVector end = start + collisionBox.GetBox().X * listenerActor->GetActorForwardVector();

	//FCollisionQueryParams QueryParams;
	//QueryParams.AddIgnoredActor(listenerActor);
	//GetWorld()->SweepMultiByChannel(out_Hits, start, end, FQuat(), ECollisionChannel::ECC_Visibility, collisionBox, QueryParams);
	
	GetWorld()->SweepMultiByChannel(out_Hits, start, end, listenerActor->GetActorRotation().Quaternion(), ECollisionChannel::ECC_Visibility, collisionBox);

	if (sweepProperties.bDisplayDebugCylinder)
	{
		FQuat rot = listenerActor->GetActorRotation().Quaternion();
		FVector b = rot.RotateVector(collisionBox.GetExtent());
		DrawDebugBox(GetWorld(), start, b, FColor::Blue, false, 2.0f, 1, 2.0f);
	}

	//for (int i = -1; i < 2; i++)
	//{
	//	FHitResult hit;
	//	FVector start = listenerActor->GetActorLocation() + listenerActor->GetActorForwardVector() * sweepProperties.StartCylinderOffset;
	//	FRotator rot = FRotator(30* i, 0, 0);
	//	//FQuat rotLocal = listenerActor->GetTransform().InverseTransformRotation(rot.Quaternion());
	//	FVector dir = rot.RotateVector(listenerActor->GetVelocity().GetSafeNormal());
	//	FVector end = start + dir * sweepProperties.DetectionLength;
	//	GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_Visibility);
	//	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.05f, 0, 1.0f);

	//	out_Hits.Add(hit);

	//}

}

UAISense_NoseSight::FDigestedNoseSightProperties::FDigestedNoseSightProperties()
{
	// init
	DetectionLength = 400.0f;
	StartCylinderOffset = 150.0f;
	bDisplayDebugCylinder = false;
}

UAISense_NoseSight::FDigestedNoseSightProperties::FDigestedNoseSightProperties(const UAISenseConfig_NoseSight& senseConfig)
{
	//Copy constructor
	DetectionLength = senseConfig.DetectionLength;
	bDisplayDebugCylinder = senseConfig.bDisplayDebugCylinder;
	StartCylinderOffset = senseConfig.StartCylinderOffset;
}
