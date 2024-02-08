// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PlaneBTTask_GroundAvoidance.h"
#include "PlanePawnAI.h"
#include "AIController.h"
#include "Physics/AircraftPhysics.h"
#include "BehaviorTree/BlackboardComponent.h"
UPlaneBTTask_GroundAvoidance::UPlaneBTTask_GroundAvoidance()
{
	NodeName = TEXT("Avoid Ground");
}

EBTNodeResult::Type UPlaneBTTask_GroundAvoidance::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	AAIController* aiController = Cast<AAIController>(ownerComp.GetAIOwner());
	APlanePawnAI* ownerPawn = Cast<APlanePawnAI>(aiController->GetPawn());

	if (ownerPawn)
	{
		FVector input = AvoidGround(ownerPawn);
		ownerPawn->GetPlanePhysicsComponent()->UpdateControlInput(input);
		//ownerPawn->GetPlanePhysicsComponent()->SetThrottleInput(1);
		aiController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, input);
	}

	FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UPlaneBTTask_GroundAvoidance::GetStaticDescription() const
{
	return FString::Printf(TEXT("Input: %s"), *BlackboardKey.SelectedKeyName.ToString());
}

FVector UPlaneBTTask_GroundAvoidance::AvoidGround(APawn* ownerPawn)
{
	float roll = ownerPawn->GetActorRotation().Roll;
	if (roll > 180.f)
		roll -= 360.f;

	roll = FMath::Clamp(roll * RollFactor, -1.0f, 1.0f);
	return FVector(roll, -1, 0);
}
