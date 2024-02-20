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

	void SetWidgetColor(const FLinearColor& color);
	void SetWidgetVisibility(ESlateVisibility visibiliy);
	ESlateVisibility GetWidgetVisibility();

protected:
	void BeginPlay() override;

	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void PlaneDeath(AActor* instigator) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Obstacle Detection")
		TObjectPtr<class UBoxComponent> DetectionVolume;
	UPROPERTY(EditAnywhere, Category = "UI", BlueprintReadWrite)
		TObjectPtr<class UWidgetComponent> WidgetComponent;

};
