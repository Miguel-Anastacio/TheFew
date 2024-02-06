// Fill out your copyright notice in the Description page of Project Settings.
#include "PlaneAIController.h"
#include "PlanePawnAI.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../TheFewProject.h"
#include "Kismet/KismetMathLibrary.h"
#include "Physics/AircraftPhysics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

APlaneAIController::APlaneAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception");
	SetPerceptionComponent(*AIPerceptionComponent.Get());

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("Behavior Tree");
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	//ActionsComponent = CreateDefaultSubobject<UAIPerceptionComponent>("Actions");
}

void APlaneAIController::BeginPlay()
{
	Super::BeginPlay();
	ControlledPlanePawn = Cast<APlanePawnAI>(GetPawn());
	PlayerPlanePawn = Cast<APlanePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (IsValid(BehaviourTree.Get()))
	{
		RunBehaviorTree(BehaviourTree.Get());
		BehaviorTreeComponent->StartTree(*BehaviourTree.Get());
	}

}

void APlaneAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	if (IsValid(Blackboard.Get()) && IsValid(BehaviourTree.Get()))
	{
		Blackboard->InitializeBlackboard(*BehaviourTree->BlackboardAsset.Get());
	}
}

void APlaneAIController::SwitchState(AI_STATE state)
{
	switch (state)
	{
	case CHASING:
		break;
	case PATROLLING:
		break;
	case AVOIDING_OBSTACLES:
		break;
	default:
		break;
	}

	CurrentState = state;
}

bool APlaneAIController::DetectObstacles(FVector& out_PawnToObstacle)
{
	return false;
}

void APlaneAIController::ChasingAction(FVector targetPosition)
{
	TargetInput = FVector::ZeroVector;
	FQuat invRotation = ControlledPlanePawn->GetActorRotation().GetInverse().Quaternion();
	FVector difference = targetPosition - ControlledPlanePawn->GetActorLocation();
	difference = invRotation.RotateVector(difference);

	// determine pitch
	FVector pitchNecessary = FVector(difference.X, 0, difference.Z).GetSafeNormal();
	//FTransform transform = ControlledPlanePawn->GetTransform();
	//FVector  pitchNecessaryGlobal = transform.TransformVector(pitchNecessary);
	float pitch = FMath::RadiansToDegrees(SignedAngle(FVector::ForwardVector, pitchNecessary, FVector::RightVector));

	if (pitch > PitchUpThreshold)
		pitch -= 360.f;


	TargetInput.Y = pitch * PitchFactor;

	// determine roll and yaw
	FVector rollNecessary = FVector(0, difference.Y, difference.Z).GetSafeNormal();
	// yaw is only used if the plane is already almost pointing to the target
	float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::ForwardVector.Dot(difference.GetSafeNormal())));
	if (angle < FineSteeringAngle)
	{
		TargetInput.Z = difference.GetSafeNormal().Y;
	}
	else
	{
		float roll = FMath::RadiansToDegrees(SignedAngle(FVector::UpVector, rollNecessary, FVector::ForwardVector));
		TargetInput.X = roll * RollFactor;
	}
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, FString::Printf(TEXT("Roll Input AI = %f"), TargetInput.X), true);
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Pitch Input AI = %f"), TargetInput.Y), true);
	GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Yellow, FString::Printf(TEXT("Yaw Input AI = %f"), TargetInput.Z), true);
	GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Yellow, FString::Printf(TEXT("Angle = %f"), angle), true);



	TargetInput.X = FMath::Clamp(TargetInput.X, -1.0f, 1.0f);


	TargetInput.Y = FMath::Clamp(TargetInput.Y, -1.0f, 1.0f);
	TargetInput.Z = FMath::Clamp(TargetInput.Z, -1.0f, 1.0f);

	ControlledPlanePawn->GetPlanePhysicsComponent()->UpdateControlInput(TargetInput);

	//UE_LOG(LogProjectFew, Log, TEXT("Picth Input = %f"));
	//DrawDebugLine(GetWorld(), PlayerPlanePawn->GetActorLocation(), PlayerPlanePawn->GetActorLocation() + pitchNecessary * -1000.0f, FColor::Black, false, 0.05f);
	ControlledPlanePawn->GetPlanePhysicsComponent()->SetThrottleInput(1);
}

void APlaneAIController::AvoidingObstacles(FVector pawnToObstacle)
{
}

float APlaneAIController::SignedAngle(FVector from, FVector to, FVector axis)
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


	//if (cross.Y > 0)
	//	return FMath::Acos(from.Dot(to));
	//else
	//	return -FMath::Acos(from.Dot(to));


}
void APlaneAIController::PatrollingAction()
{

}

void APlaneAIController::Tick(float dt)
{
	/*FVector target = PlayerPlanePawn->GetActorLocation();
	switch (CurrentState)
	{
	case CHASING:
		ChasingAction(target);
		break;
	case PATROLLING:
		PatrollingAction();
		break;
	case AVOIDING_OBSTACLES:
		AvoidingObstacles(PawnToObstacle);
		break;
	default:
		break;
	}

	if (DetectObstacles(PawnToObstacle))
	{
		SwitchState(AVOIDING_OBSTACLES);
	}*/
}

