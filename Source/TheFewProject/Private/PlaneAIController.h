// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "PlaneAIController.generated.h"

UENUM(BlueprintType)
enum AI_STATE
{
	CHASING UMETA(DisplayName = "Chasing"),
	PATROLLING   UMETA(DisplayName = "Patrolling"),
	AVOIDING_OBSTACLES      UMETA(DisplayName = "Obstacle Avoiding"),
};

/**
 * 
 */
UCLASS()
class APlaneAIController : public AAIController
{
	GENERATED_BODY()
public:
	APlaneAIController();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* pawn) override;
	virtual void BeginPlay() override;

	void SwitchState(AI_STATE new_state);
	bool DetectObstacles(FVector& out_PawnToObstacle);
	void PatrollingAction();
	void ChasingAction(FVector targetPosition);
	void AvoidingObstacles(FVector pawnToObstacle);
	
	float SignedAngle(FVector from, FVector to, FVector axis);


	AI_STATE CurrentState = CHASING;
	FVector PawnToObstacle;
	FVector TargetInput;
	TObjectPtr<class APlanePawnAI> ControlledPlanePawn;
	TObjectPtr<class APlanePawn> PlayerPlanePawn;

	UPROPERTY(EditAnywhere, Category = "Controls")
		float PitchUpThreshold;
	UPROPERTY(EditAnywhere, Category = "Controls")
		float FineSteeringAngle;
	UPROPERTY(EditAnywhere, Category = "Controls")
		float RollFactor = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Controls")
		float PitchFactor = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		TObjectPtr<UBehaviorTree> BehaviourTree;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		TObjectPtr<class UBehaviorTreeComponent> BehaviorTreeComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		TObjectPtr<UBlackboardComponent> BlackboardComponent;
	UPROPERTY(EditAnywhere, Category = "Perception")
		TObjectPtr<class UAIPerceptionComponent> AIPerceptionComponent;
	//UPROPERTY(EditAnywhere, Category = "Actions")
	//	TObjectPtr<class UAIPerceptionComponent> ActionsComponent;
};
