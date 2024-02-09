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
	GAINING_ALTITUDE      UMETA(DisplayName = "Gaining Altitude"),
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
	FORCEINLINE void SetTargetActor(AActor* target) { TargetActor = target; };
	void ShowDebugInfo(FVector input);
	void ShowDebugInfo();
protected:
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* pawn) override;
	virtual void BeginPlay() override;

	void SwitchState(AI_STATE new_state);
	bool DetectObstacles(FVector& out_PawnToObstacle);
	void PatrollingAction();
	FVector SteerToTarget(const FVector& targetPosition, APawn* ownerPawn);
	FVector RecoverAltitude(APawn* pawn);
	FVector AvoidGround(APawn* pawnToObstacle);
	
	bool IsPlaneFacingTarget(AActor* target);
	bool DetectObstacles(AActor* pawn);

	float SignedAngle(FVector from, FVector to, FVector axis);
	float Angle(FVector from, FVector to);


protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> TargetActor;
	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<AI_STATE> CurrentState = CHASING;
	FVector PawnToObstacle;
	FVector TargetInput;

	TObjectPtr<class APlanePawnAI> ControlledPlanePawn;
	//TObjectPtr<AActor> TargetActor;

	UPROPERTY(EditAnywhere, Category = "Controls")
		float PitchUpThreshold;
	UPROPERTY(EditAnywhere, Category = "Controls")
		float FineSteeringAngle;
	UPROPERTY(EditAnywhere, Category = "Controls")
		float RollFactor = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Controls")
		float PitchFactor = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Controls")
		float MinAltitude = 75.0f;

	UPROPERTY(EditAnywhere, Category = "Controls")
		float AngleRange = 10;


	UPROPERTY(EditAnywhere, Category = "Detection")
		float LengthOfRays = 2000.f;;
	UPROPERTY(EditAnywhere, Category = "Detection")
		float StartPosOffSet = 500.f;
	UPROPERTY(EditAnywhere, Category = "Detection")
		float AngleBetweenRays = 30.f;
	UPROPERTY(EditAnywhere, Category = "Detection")
		int NumberOfRaysPerAxis = 5.0f;

	float Altitude = 0.f;
	// NOT IN USE
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		TObjectPtr<UBehaviorTree> BehaviourTree;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		TObjectPtr<class UBehaviorTreeComponent> BehaviorTreeComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		TObjectPtr<UBlackboardComponent> BlackboardComponent;
	UPROPERTY(EditAnywhere, Category = "Perception")
		TObjectPtr<class UAIPerceptionComponent> AIPerceptionComponent;
	UPROPERTY(EditAnywhere, Category = "Controls")
		float GroundAvoidanceMinSpeed = 75.0f;

	//UPROPERTY(EditAnywhere, Category = "Controls")
	//	float MinVelocity = 75.0f;

	//UPROPERTY(EditAnywhere, Category = "Controls")
	//	float GroundAvoidanceMaxSpeed = 75.0f;
	//UFUNCTION()
	//	void UpdateBlackboardKeys(AActor* actor, FAIStimulus stimulus);
	UFUNCTION()
		void UpdateBlackboardKeys(const TArray<AActor*>& actors);
	//UPROPERTY(EditAnywhere, Category = "Actions")
	//	TObjectPtr<class UAIPerceptionComponent> ActionsComponent;

	UFUNCTION(BlueprintCallable)
		bool IsAtLowAltitude();
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
