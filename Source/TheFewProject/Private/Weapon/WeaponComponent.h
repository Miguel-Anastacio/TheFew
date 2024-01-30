// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponComponent.generated.h"

class AProjectile;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWeaponComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void FireBullet();
	void SetActorForwardVector(FVector vector) {
		ActorForwardVector = vector;
	};
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, Category = "Bullet")
		TSubclassOf<AProjectile> BulletTemplate;


	// audio for firing
	// firing vfx


	UPROPERTY(EditAnywhere, Category = "Gun")
		float RateOfFire = 60.0f;
	UPROPERTY(EditAnywhere, Category = "Gun")
		float ReloadTime = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Gun")
		float DistanceFromNose = 0.0f;

	float Timer = 1000.0f;
	float TimeBetweenShots = 0.0f;
	FVector ActorForwardVector;

	TArray<AProjectile*> BulletManager;



		
};
