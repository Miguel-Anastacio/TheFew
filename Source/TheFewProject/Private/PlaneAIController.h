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
	FORCEINLINE AActor* GetTargetActor() { return TargetActor; };
	FString GetTargetActorName();
	void ShowDebugInfo(FVector input);
	void ShowDebugInfo();

	FORCEINLINE void SetPatrolDestination(AActor* dest) { PatrolDestination = dest; };
protected:
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* pawn) override;
	virtual void BeginPlay() override;

	
	//bool DetectObstacles(FVector& out_PawnToObstacle);
	void SwitchState(AI_STATE new_state);
	void PatrollingAction();
	FVector SteerToTarget(const FVector& targetPosition, APawn* ownerPawn);
	FVector RecoverAltitude(APawn* pawn);
	FVector AvoidTerrain(APawn* pawnToObstacle, float rollDirection, float rollTarget = 0);
	
	bool IsPlaneFacingTarget(AActor* target);
	// returns the roll direction to avoid the obstacle and the roll angle target 
	FVector2D DetectObstacles(AActor* pawn);

	float UpdateThrottle(AActor* pawn);

	float SignedAngle(FVector from, FVector to, FVector axis);
	float Angle(FVector from, FVector to);

	// ray detection and response functions

	void PerformSweep(const FVector& forward, const FVector& right, const FVector& start, 
		TFunction<void(const FVector&, const FVector&, const FVector&, const float, float&, float&)> SweepResponse,	float& out_leftHits, float& out_rightHits);
	void PerformSweep(const FVector& forward, const FVector& right, const FVector& start, float& out_leftHits, float& out_rightHits);
	void GroundResponse(const FVector& dir, const FVector& right, const FVector& forward, const float angle, float& out_leftHits, float& out_rightHits);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> TargetActor;
	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<AI_STATE> CurrentState = CHASING;


	FVector PawnToObstacle;
	FVector TargetInput;
	

	TObjectPtr<class APlanePawnAI> ControlledPlanePawn;
	//TObjectPtr<AActor> TargetActor;

	UPROPERTY(EditAnywhere, Category = "Patrol")
		TObjectPtr<AActor> PatrolDestination;
	UPROPERTY(EditAnywhere, Category = "Patrol")
		float MaxPatrolTime = 7.5f;

	float PatrolTimer = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Controls")
		float PitchUpThreshold;
	UPROPERTY(EditAnywhere, Category = "Controls")
		float FineSteeringAngle;
	UPROPERTY(EditAnywhere, Category = "Controls")
		float RollFactor = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Controls")
		float RollFactorEmergency = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Controls")
		float PitchFactor = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Controls")
		float MinAltitude = 75.0f;
	UPROPERTY(EditAnywhere, Category = "Controls")
		float AngleRange = 10;
	UPROPERTY(EditAnywhere, Category = "Guns")
		float DistanceFiring = 1200.f;
	UPROPERTY(EditAnywhere, Category = "Throttle")
		float MinSpeed = 700.f;
	UPROPERTY(EditAnywhere, Category = "Throttle")
		float DistanceTriggerBrakes = 700.f;

	UPROPERTY(EditAnywhere, Category = "Detection")
		float LengthOfRays = 2000.f;;
	UPROPERTY(EditAnywhere, Category = "Detection")
		float StartPosOffSet = 500.f;
	UPROPERTY(EditAnywhere, Category = "Detection")
		float AngleBetweenRays = 30.f;
	UPROPERTY(EditAnywhere, Category = "Detection")
		float AngleCovered = 30.f;
	UPROPERTY(EditAnywhere, Category = "Detection")
		int NumberOfRaysPerAxis = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Target")
		float MinimumTimeFollowingTarget = 5.0f;
	float TimerChangeTargets = 100000.0f;

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


	friend class APlanePhysicsDebugHUD;
};
