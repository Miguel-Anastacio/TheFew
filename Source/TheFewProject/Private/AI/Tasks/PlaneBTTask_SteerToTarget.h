// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "PlaneBTTask_SteerToTarget.generated.h"

/**
 * Task that calculates input necessary to steer plane towards target 
 */
UCLASS(Blueprintable)
class UPlaneBTTask_SteerToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UPlaneBTTask_SteerToTarget();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
	FVector CalculateInput(const FVector& targetPosition, APawn* ownerPawn);


	float SignedAngle(FVector from, FVector to, FVector axis);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls", meta = (AllowPrivateAccess = true))
		float PitchUpThreshold = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls", meta = (AllowPrivateAccess = true))
		float FineSteeringAngle = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls", meta = (AllowPrivateAccess = true))
		float RollFactor = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls", meta = (AllowPrivateAccess = true))
		float PitchFactor = 1.0f;

};
