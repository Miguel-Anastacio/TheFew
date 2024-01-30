// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlanePawn.generated.h"

class UBoxComponent;
class UCapsuleComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UAircraftPhysics;
class UWeaponComponent;

UCLASS()
class APlanePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlanePawn();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//float GetThrust() { return Thrust; };
	//void SetThrust(float t) { Thrust = t; };
	//void IncreaseThrust() { Thrust += 1; };

	void SwapCamera();
	void MoveCamera(FVector2D input);
	void ResetCameraLerpTimer() {
		rotTimer = 0;
	};

	void ToggleLandingGear();
	UAircraftPhysics* GetPlanePhysicsComponent() { return PlanePhysicsComponent; };
	void TriggerWeapons();
	bool GetIsFlying() { return Flying; };
	

protected:
	virtual void BeginPlay() override;
	void CreateMeshWithPivot(USceneComponent* pivot, UStaticMeshComponent* mesh, FName namePivot, FName nameMesh);
	void AnimateControlSurface(float input, USceneComponent* surfacePivot, FRotator axis, float dt);
	void UpdateFlying();

protected:
	UPROPERTY(EditAnywhere )
		USceneComponent* PlaneRoot;
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* BackWheel;
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* LeftWheelCollider;
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* RightWheelCollider;
	UPROPERTY(EditAnywhere)
		UBoxComponent* PlaneBodyBox;

	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* PlaneBodyMesh;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* PlanePropellerMesh;
	UPROPERTY(EditAnywhere)
		USceneComponent* PropellerRoot;

	UPROPERTY(EditAnywhere)
		USceneComponent* RudderRoot;
	UPROPERTY(EditAnywhere)
		USceneComponent* ElevatorRoot;
	UPROPERTY(EditAnywhere)
		USceneComponent* AileronLeftRoot;
	UPROPERTY(EditAnywhere)
		USceneComponent* AileronRightRoot;
	UPROPERTY(EditAnywhere)
		USceneComponent* LeftLandingGearRoot;
	UPROPERTY(EditAnywhere)
		USceneComponent* RightLandingGearRoot;

	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* AileronLeftMesh;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* AileronRightMesh;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* ElevatorMesh;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent*RudderMesh;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* LeftLandingGearMesh;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* RightLandingGearMesh;

	UPROPERTY(EditAnywhere, Category = "Tail Camera")
		USpringArmComponent* TailCameraBoom;
	UPROPERTY(EditAnywhere, Category = "Tail Camera")
		UCameraComponent* TailCamera;
	UPROPERTY(EditAnywhere, Category = "Propeller Camera")
		USpringArmComponent* PropellerCameraBoom;
	UPROPERTY(EditAnywhere, Category = "Propeller Camera")
		UCameraComponent* PropellerCamera;

	UPROPERTY(EditAnywhere, Category = "Physics")
		UAircraftPhysics* PlanePhysicsComponent;

	UPROPERTY(EditAnywhere, Category = "Weapon")
		UWeaponComponent* LeftWeaponComponent;
	UPROPERTY(EditAnywhere, Category = "Weapon")
		UWeaponComponent* RightWeaponComponent;

	UPROPERTY(EditAnywhere, Category = "Plane Animation")
		float PropellerRotationSpeed = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Plane Animation")
		float TargetAngle = 15.0f;
	UPROPERTY(EditAnywhere, Category = "Plane Animation")
		float ControlSurfacesAnimationSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float CameraMoveSpeed = 50.0f;

	// how much can the camera boom rotate in the Y and Z axis
	UPROPERTY(EditAnywhere, Category = "Camera")
		FVector2D MaxLookAngle = FVector2D(90, 180);

	UPROPERTY(EditAnywhere, Category = "Landing Gear")
		float DeploySpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Landing Gear")
	bool LandingGear = false;

	UPROPERTY(VisibleAnywhere)
	bool Flying = false;

	FVector2D CameraInput = FVector2D(0, 0);
	FRotator CameraRotDirection = FRotator(0, 0,0);
	FQuat TargetCameraRotation;
	float rotTimer = 0.0f;
	FRotator DefaultCameraRotation;

	FRotator LandingGearTargetRotation;
	

	friend class APlanePhysicsDebugHUD;
};
