// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlanePawn.h"
#include "PlanePawnAI.generated.h"

/**
 * 
 */
UCLASS()
class APlanePawnAI : public APlanePawn
{
	GENERATED_BODY()
public:
	APlanePawnAI();
	TObjectPtr<class UBoxComponent> GetDetectionVolume();
	void Tick(float deltaTime) override;
protected:
	void BeginPlay() override;
	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	UPROPERTY(EditAnywhere, Category = "Obstacle Detection")
		TObjectPtr<class UBoxComponent> DetectionVolume;

};
