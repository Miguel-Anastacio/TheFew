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
#include "Components/BoxComponent.h"

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
	PlayerPlanePawn = Cast<APlanePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (IsValid(BehaviourTree.Get()))
	{
		//RunBehaviorTree(BehaviourTree.Get());
		//BehaviorTreeComponent->StartTree(*BehaviourTree.Get());
	}


}

void APlaneAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	ControlledPlanePawn = Cast<APlanePawnAI>(GetPawn());

	if (ControlledPlanePawn)
	{
		ControlledPlanePawn->GetPlanePhysicsComponent()->SetThrottleInput(1);
	}
	if (IsValid(Blackboard.Get()) && IsValid(BehaviourTree.Get()))
	{
		Blackboard->InitializeBlackboard(*BehaviourTree->BlackboardAsset.Get());
	}

	if (IsValid(AIPerceptionComponent.Get()))
	{
		//AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &APlaneAIController::UpdateBlackboardKeys);
		AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &APlaneAIController::UpdateBlackboardKeys);
	}

	UBoxComponent* volume = ControlledPlanePawn->GetDetectionVolume().Get();
	volume->OnComponentBeginOverlap.AddDynamic(this, &APlaneAIController::OnOverlapBegin);
	volume->OnComponentEndOverlap.AddDynamic(this, &APlaneAIController::OnOverlapEnd);
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
	case GAINING_ALTITUDE:
		if (CurrentState == AVOIDING_OBSTACLES)
			return;
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

FVector APlaneAIController::SteerToTarget(const FVector& targetPosition, APawn* ownerPawn)
{
	FVector targetInput = FVector::ZeroVector;

	FQuat invRotation = ownerPawn->GetActorRotation().GetInverse().Quaternion();

	FVector difference = targetPosition - ownerPawn->GetActorLocation();
	difference = invRotation.RotateVector(difference);

	// determine pitch
	FVector pitchNecessary = FVector(difference.X, 0, difference.Z).GetSafeNormal();
	float pitch = FMath::RadiansToDegrees(SignedAngle(FVector::ForwardVector, pitchNecessary, FVector::RightVector));

	if (pitch > PitchUpThreshold)
		pitch -= 360.f;

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

FVector APlaneAIController::AvoidGround(APawn* pawn)
{
	float roll = pawn->GetActorRotation().Roll;
	if (roll > 180.f)
		roll -= 360.f;

	roll = FMath::Clamp(roll * RollFactor, -1.0f, 1.0f);
	return FVector(roll, -1, 0);
}

FVector APlaneAIController::RecoverAltitude(APawn* pawn)
{
	float targetPitch = 30;

	float roll = pawn->GetActorRotation().Roll;
	if (roll > 180.f)
		roll -= 360.f;

	// determine pitch
	float currentPitch = pawn->GetActorRotation().Pitch;
	//currentPitch = 0.0f;
	float pitch = targetPitch - currentPitch;

	if (currentPitch > targetPitch)
		pitch = 0;

	roll = FMath::Clamp(roll * RollFactor, -1.0f, 1.0f);
	pitch = FMath::Clamp(-pitch * PitchFactor, -1.0f, 1.0f);
	return FVector(roll, pitch, 0);
}


bool APlaneAIController::IsPlaneFacingTarget(APawn* pawn)
{
	FVector targetPosition = pawn->GetActorLocation();
	FVector error = targetPosition - ControlledPlanePawn->GetActorLocation();
	float range = error.Size();
	FVector targetDir = error.GetSafeNormal();
	//float targetAngle = FVector.Angle(targetDir, plane.Rigidbody.rotation * Vector3.forward);
	//float angle = Angle(pawn->GetActorTransform().TransformVector(FVector::ForwardVector), targetDir);
	float angle = Angle(ControlledPlanePawn->GetActorForwardVector(), targetDir);
	//DrawDebugLine(GetWorld(), ControlledPlanePawn->GetActorLocation(), ControlledPlanePawn->GetActorLocation() + pawn->GetActorRotation().RotateVector(FVector::ForwardVector) * MinAltitude, FColor::Blue, false, 0.05f, 0, 1.0f);
	//DrawDebugLine(GetWorld(), ControlledPlanePawn->GetActorLocation(), ControlledPlanePawn->GetActorLocation() + ControlledPlanePawn->GetActorForwardVector() * MinAltitude, FColor::Green, false, 0.05f, 0, 1.0f);
	if (abs(angle) < AngleRange)
	{
		//ControlledPlanePawn->TriggerWeapons();
		//fire cannon
		GEngine->AddOnScreenDebugMessage(9, 1.0f, FColor::Yellow, FString::Printf(TEXT("Firing")), true);
		return true;
	}
	return false;
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

float APlaneAIController::Angle(FVector from, FVector to)
{
	from.Normalize();
	to.Normalize();


	float unsignedAngle = FMath::Acos(from.Dot(to));
	return FMath::RadiansToDegrees(unsignedAngle);


	//if (cross.Y > 0)
	//	return FMath::Acos(from.Dot(to));
	//else
	//	return -FMath::Acos(from.Dot(to));


}
void APlaneAIController::UpdateBlackboardKeys(const TArray<AActor*>& actors)
{
	Blackboard->SetValueAsBool(FName("GroundDetected"), false);
	for (const auto& act : actors)
	{
		if (act->ActorHasTag(FName("Terrain")))
			Blackboard->SetValueAsBool(FName("GroundDetected"), true);
	}
}

//}
//void APlaneAIController::UpdateBlackboardKeys(AActor* actor, FAIStimulus stimulus)
//{
//	if (!actor)
//	{
//		Blackboard->SetValueAsBool(FName("GroundDetected"), false);
//	}
//
//	if(actor->ActorHasTag(FName("Terrain")))
//		Blackboard->SetValueAsBool(FName("GroundDetected"), true);
//}
void APlaneAIController::PatrollingAction()
{

}

void APlaneAIController::Tick(float dt)
{
	/*FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(ControlledPlanePawn);
	GetWorld()->LineTraceSingleByChannel(hit, ControlledPlanePawn->GetActorLocation(), ControlledPlanePawn->GetActorLocation() + FVector::DownVector * MinAltitude, ECC_WorldStatic, params);
	DrawDebugLine(GetWorld(), ControlledPlanePawn->GetActorLocation(), ControlledPlanePawn->GetActorLocation() + FVector::DownVector * MinAltitude, FColor::Red, false, 0.05f, 0, 1.0f);
	if (hit.GetActor())
	{
		Blackboard->SetValueAsBool(FName("LowAltitude"), true);
	}
	else
	{
		Blackboard->SetValueAsBool(FName("LowAltitude"), false);
	}*/

	FRotator rot = ControlledPlanePawn->GetActorRotation();
	FVector temp = FVector(rot.Roll, rot.Pitch, rot.Yaw);
	/*AddVector(TEXT("Player Rotation"), temp);*/
	GEngine->AddOnScreenDebugMessage(4, 31.0f, FColor::Yellow, FString::Printf(TEXT("Roll AI = %f"), rot.Roll), true);
	GEngine->AddOnScreenDebugMessage(5, 31.0f, FColor::Yellow, FString::Printf(TEXT("Pitch AI = %f"), rot.Pitch), true);
	GEngine->AddOnScreenDebugMessage(6, 31.0f, FColor::Yellow, FString::Printf(TEXT("Yaw = %f"), rot.Yaw), true);
	GEngine->AddOnScreenDebugMessage(10, 31.0f, FColor::Yellow, FString::Printf(TEXT("Throttle = %f"), 
		ControlledPlanePawn->GetPlanePhysicsComponent()->GetThrottle()), true);
	GEngine->AddOnScreenDebugMessage(11, 31.0f, FColor::Yellow, FString::Printf(TEXT("Velocity = %f"),
		ControlledPlanePawn->GetVelocity().Size()), true);	
	GEngine->AddOnScreenDebugMessage(12, 31.0f, FColor::Yellow, FString::Printf(TEXT("Altitude = %f"),
		ControlledPlanePawn->GetActorLocation().Z / 100.f), true);
	//GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Yellow, FString::Printf(TEXT("Angle = %f"), angle), true);
	DrawDebugLine(GetWorld(), ControlledPlanePawn->GetActorLocation(), ControlledPlanePawn->GetActorLocation() + ControlledPlanePawn->GetVelocity(), FColor::Green, false, 0.05f, 0, 1.0f);

	FVector target = PlayerPlanePawn->GetActorLocation();
	FVector input;
	//CurrentState = GAINING_ALTITUDE;
	switch (CurrentState)
	{
	case CHASING:
		input = SteerToTarget(target, ControlledPlanePawn);
		GEngine->AddOnScreenDebugMessage(7, 1.0f, FColor::Blue,  FString::Printf(TEXT("CHASING")), true);
		break;
	case PATROLLING:
		PatrollingAction();
		break;
	case AVOIDING_OBSTACLES:
		input = AvoidGround(ControlledPlanePawn);
		GEngine->AddOnScreenDebugMessage(7, 1.0f, FColor::Blue, FString::Printf(TEXT("Avoid Ground")), true);
		break;
	case GAINING_ALTITUDE:
		input = RecoverAltitude(ControlledPlanePawn);
		GEngine->AddOnScreenDebugMessage(7, 1.0f, FColor::Blue, FString::Printf(TEXT("Recover Altitude")), true);
		if (!IsAtLowAltitude())
		{
			SwitchState(CHASING);
		}

	default:
		break;
	}
	ControlledPlanePawn->GetPlanePhysicsComponent()->UpdateControlInput(input);

	if (IsAtLowAltitude())
	{
		SwitchState(GAINING_ALTITUDE);
	}
	if (IsPlaneFacingTarget(PlayerPlanePawn))
	{
		ControlledPlanePawn->TriggerWeapons();
	}
}

bool APlaneAIController::IsAtLowAltitude()
{
	FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(ControlledPlanePawn);
	GetWorld()->LineTraceSingleByChannel(hit, ControlledPlanePawn->GetActorLocation(), ControlledPlanePawn->GetActorLocation() + FVector::DownVector * MinAltitude, ECC_WorldStatic, params);
	DrawDebugLine(GetWorld(), ControlledPlanePawn->GetActorLocation(), ControlledPlanePawn->GetActorLocation() + FVector::DownVector * MinAltitude, FColor::Red, false, 0.05f, 0, 1.0f);
	if (hit.GetActor())
	{
		Blackboard->SetValueAsBool(FName("LowAltitude"), true);
		return true;
	}
	else
	{
		Blackboard->SetValueAsBool(FName("LowAltitude"), false);
		return false;
	}
}

void APlaneAIController::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*if (OtherActor->ActorHasTag(FName("Terrain")))
	{
		Blackboard->SetValueAsBool(FName("GroundDetected"), true);
		ControlledPlanePawn->GetPlanePhysicsComponent()->SetThrottleInput(-1);
		SwitchState(AVOIDING_OBSTACLES);
	}*/
}

void APlaneAIController::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*if (OtherActor->ActorHasTag(FName("Terrain")))
	{
		Blackboard->SetValueAsBool(FName("GroundDetected"), false);
		ControlledPlanePawn->GetPlanePhysicsComponent()->SetThrottleInput(1);
		SwitchState(CHASING);
	}*/
}