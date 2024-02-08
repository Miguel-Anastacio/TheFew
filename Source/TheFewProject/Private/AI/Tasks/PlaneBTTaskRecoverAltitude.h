// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "PlaneBTTaskRecoverAltitude.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UPlaneBTTaskRecoverAltitude : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UPlaneBTTaskRecoverAltitude();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
	FVector RecoverAltitude(APawn* ownerPawn);

	//float SignedAngle(FVector from, FVector to, FVector axis);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls", meta = (AllowPrivateAccess = true))
		float RollFactor = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls", meta = (AllowPrivateAccess = true))
		float PitchFactor = 1.0f;
};
