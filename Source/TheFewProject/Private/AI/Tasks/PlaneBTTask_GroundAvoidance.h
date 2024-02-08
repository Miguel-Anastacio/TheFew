// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "PlaneBTTask_GroundAvoidance.generated.h"

/**
 * Task that is performed when the ground is detected by the nose sight sense
 */
UCLASS(Blueprintable)
class UPlaneBTTask_GroundAvoidance : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UPlaneBTTask_GroundAvoidance();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
	FVector AvoidGround(APawn* ownerPawn);

	//float SignedAngle(FVector from, FVector to, FVector axis);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls", meta = (AllowPrivateAccess = true))
		float RollFactor = 0.01f;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls", meta = (AllowPrivateAccess = true))
	//	float PitchFactor = 1.0f;
};
