// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlanePawn.h"
#include "PlanePawnPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyHitSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSpawnSignature, AActor*, player);
/**
 * 
 */
UCLASS()
class APlanePawnPlayer : public APlanePawn
{
	GENERATED_BODY()
	
public:
	//APlanePawnPlayer();
	void PostInitializeComponents() override;
	void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void RespawnPlayer(const FVector& spawnLocation);
	void RespawnPlayer();
	void PlaneDeathSimple() override;
public:
	FOnEnemyHitSignature EnemyHitDelegate;
	FOnPlayerSpawnSignature PlayerSpawnDelegate;
protected:
	void Tick(float dt) override;
	void PlaneDeath(AActor* instigator) override;
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult) override;

	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	//UPROPERTY(EditAnywhere, Category = "UI")
	//	TObjectPtr<class  UWidgetInteractionComponent> WidgetInteractionComponent;
	UPROPERTY(EditAnywhere)
		FVector InitialVelocity = FVector(1000, 0, 600);
	FTimerHandle OutOfBoundsTimerHandle;

	FRotator InitalRotation;
	FVector InitalLocation;

	TMap<FString, class UPrimitiveComponent*> OverlappedBounds;

};
