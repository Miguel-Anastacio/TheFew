// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PlaneBTTask_SteerToTarget.h"
#include "PlaneBTTask_SteerToTarget.h"
#include "PlanePawnAI.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Physics/AircraftPhysics.h"
#include "BehaviorTree/BlackboardComponent.h"
UPlaneBTTask_SteerToTarget::UPlaneBTTask_SteerToTarget()
{
	bNotifyTick = true;
	NodeName = TEXT("Update Steering Input");
}

EBTNodeResult::Type UPlaneBTTask_SteerToTarget::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	//AAIController* aiController = Cast<AAIController>(ownerComp.GetAIOwner());
	//APlanePawnAI* ownerPawn = Cast<APlanePawnAI>(aiController->GetPawn());
	////APawn* ownerPawn = aiController->GetPawn();

	//FVector targetPos = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	//if (ownerPawn)
	//{
	//	FVector input = CalculateInput(targetPos, ownerPawn);
	//	ownerPawn->GetPlanePhysicsComponent()->UpdateControlInput(input);
	//	ownerPawn->GetPlanePhysicsComponent()->SetThrottleInput(1);
	//	aiController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, input);
	//}
	//
	//FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::InProgress;
}

void UPlaneBTTask_SteerToTarget::TickTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory, float deltaSeconds)
{
	AAIController* aiController = Cast<AAIController>(ownerComp.GetAIOwner());
	APlanePawnAI* ownerPawn = Cast<APlanePawnAI>(aiController->GetPawn());
	//APawn* ownerPawn = aiController->GetPawn();

	FVector targetPos = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	if (ownerPawn)
	{
		FVector input = CalculateInput(targetPos, ownerPawn);
		ownerPawn->GetPlanePhysicsComponent()->UpdateControlInput(input);
		ownerPawn->GetPlanePhysicsComponent()->SetThrottleInput(1);
		aiController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, input);
	}
}

FString UPlaneBTTask_SteerToTarget::GetStaticDescription() const
{
	return FString::Printf(TEXT("Input: %s"), *BlackboardKey.SelectedKeyName.ToString());
}

FVector UPlaneBTTask_SteerToTarget::CalculateInput(const FVector& targetPosition, APawn* ownerPawn)
{
	//APlanePawnAI* planePawn = Cast<APlanePawnAI>(ownerPawn);

	//FVector targetPosition = targetActor->GetActorLocation();
	FVector targetInput = FVector::ZeroVector;

	FQuat invRotation = ownerPawn->GetActorRotation().GetInverse().Quaternion();

	FVector difference = targetPosition - ownerPawn->GetActorLocation();
	difference = invRotation.RotateVector(difference);

	// determine pitch
	FVector pitchNecessary = FVector(difference.X, 0, difference.Z).GetSafeNormal();
	float pitch = FMath::RadiansToDegrees(SignedAngle(FVector::ForwardVector, pitchNecessary, FVector::RightVector));

	//if (pitch > PitchUpThreshold)
	//	pitch -= 360.f;



	float minPitch = -180;
	float maxPitch = 180;
	targetInput.Y = FMath::Lerp(-1.f, 1.0f, (pitch - minPitch) / (maxPitch - minPitch));
	targetInput.Y = pitch * PitchFactor;

	// determine roll and yaw
	FVector rollNecessary = FVector(0, difference.Y, difference.Z).GetSafeNormal();
	// yaw is only used if the plane is already almost pointing to the target
	float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::ForwardVector.Dot(difference.GetSafeNormal())));
	if (angle < FineSteeringAngle)
	{
		targetInput.Z = difference.GetSafeNormal().Y;
	}
	else
	{
		float roll = FMath::RadiansToDegrees(SignedAngle(FVector::UpVector, rollNecessary, FVector::ForwardVector));

		targetInput.X = FMath::Lerp(-1.f, 1.0f, (roll - minPitch) / (maxPitch - minPitch));
		targetInput.X = roll * RollFactor;

	}
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, FString::Printf(TEXT("Roll Input AI = %f"), targetInput.X), true);
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Pitch Input AI = %f"), targetInput.Y), true);
	GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Yellow, FString::Printf(TEXT("Yaw Input AI = %f"), targetInput.Z), true);
	GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Yellow, FString::Printf(TEXT("Angle = %f"), angle), true);

	targetInput.X = FMath::Clamp(targetInput.X, -1.0f, 1.0f);
	targetInput.Y = FMath::Clamp(targetInput.Y, -1.0f, 1.0f);
	targetInput.Z = FMath::Clamp(targetInput.Z, -1.0f, 1.0f);

	//ControlledPlanePawn->GetPlanePhysicsComponent()->UpdateControlInput(TargetInput);

	//UE_LOG(LogProjectFew, Log, TEXT("Picth Input = %f"));
	//DrawDebugLine(GetWorld(), PlayerPlanePawn->GetActorLocation(), PlayerPlanePawn->GetActorLocation() + pitchNecessary * -1000.0f, FColor::Black, false, 0.05f);
	//ControlledPlanePawn->GetPlanePhysicsComponent()->SetThrottleInput(1);
	
	return targetInput;
}

float UPlaneBTTask_SteerToTarget::SignedAngle(FVector from, FVector to, FVector axis)
{
	from.Normalize();
	to.Normalize();

	FVector cross = from.Cross(to);

	float unsignedAngle = FMath::Acos(from.Dot(to));
	float cross_x = from.Y * to.Z - from.Z * to.Y;
	float cross_y = from.Z * to.X - from.X * to.Z;
	float cross_z = from.X * to.Y - from.Y * to.X;
	float sign = FMath::Sign(axis.X * cross_x + axis.Y * cross_y + axis.Z * cross_z);
	return unsignedAngle * sign;

}
