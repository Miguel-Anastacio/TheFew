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

	UAircraftPhysics* GetPlanePhysicsComponent() { return PlanePhysicsComponent; };

protected:
	virtual void BeginPlay() override;
	void CreateMeshWithPivot(USceneComponent* pivot, UStaticMeshComponent* mesh, FName namePivot, FName nameMesh);
	void AnimateControlSurface(float input, USceneComponent* surfacePivot, FRotator axis, float dt);
protected:
	UPROPERTY(EditAnywhere )
		USceneComponent* PlaneRoot;
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* BackWheelCapsule;
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* FrontLeftWheelCapsule;
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* FrontRightWheelCapsule;
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

	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* AileronLeftMesh;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* AileronRightMesh;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* ElevatorMesh;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent*RudderMesh;

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

	FVector2D CameraInput = FVector2D(0, 0);
	FRotator CameraRotDirection = FRotator(0, 0,0);
	FQuat TargetCameraRotation;
	float rotTimer = 0.0f;
	FRotator DefaultCameraRotation;
	friend class APlanePhysicsDebugHUD;
};
