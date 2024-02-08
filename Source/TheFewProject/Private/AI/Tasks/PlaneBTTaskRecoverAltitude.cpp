// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PlaneBTTaskRecoverAltitude.h"
#include "PlanePawnAI.h"
#include "AIController.h"
#include "Physics/AircraftPhysics.h"
#include "BehaviorTree/BlackboardComponent.h"

UPlaneBTTaskRecoverAltitude::UPlaneBTTaskRecoverAltitude()
{
	NodeName = TEXT("Recover Altitude");
}

EBTNodeResult::Type UPlaneBTTaskRecoverAltitude::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	AAIController* aiController = Cast<AAIController>(ownerComp.GetAIOwner());
	APlanePawnAI* ownerPawn = Cast<APlanePawnAI>(aiController->GetPawn());

	if (ownerPawn)
	{
		FVector input = RecoverAltitude(ownerPawn);
		ownerPawn->GetPlanePhysicsComponent()->SetThrottleInput(1);
		ownerPawn->GetPlanePhysicsComponent()->UpdateControlInput(input);
		aiController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, input);
	}

	FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UPlaneBTTaskRecoverAltitude::GetStaticDescription() const
{
	return FString::Printf(TEXT("Input: %s"), *BlackboardKey.SelectedKeyName.ToString());
}

FVector UPlaneBTTaskRecoverAltitude::RecoverAltitude(APawn* ownerPawn)
{
	float targetPitch = 30;

	float roll = ownerPawn->GetActorRotation().Roll;
	if (roll > 180.f)
		roll -= 360.f;

	// determine pitch
	//FVector pitchNecessary = FVector(difference.X, 0, difference.Z).GetSafeNormal();
	//float pitch = FMath::RadiansToDegrees(SignedAngle(FVector::ForwardVector, pitchNecessary, FVector::RightVector));
	float currentPitch = ownerPawn->GetActorRotation().Pitch;
		currentPitch = 0.0f;
	float pitch = targetPitch - currentPitch;

	if (currentPitch > targetPitch)
		pitch = 0;

	roll = FMath::Clamp(roll * RollFactor, -1.0f, 1.0f);
	pitch = FMath::Clamp(-pitch * PitchFactor, -1.0f, 1.0f);
	return FVector(roll, pitch, 0);
}
