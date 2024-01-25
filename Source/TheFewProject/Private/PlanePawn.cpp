// Fill out your copyright notice in the Description page of Project Settings.

#include "PlanePawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Physics/AircraftPhysics.h"
#include "Camera/CameraComponent.h"

// Sets default values
APlanePawn::APlanePawn()
{
	PlaneRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = PlaneRoot;

	PlaneBodyBox = CreateDefaultSubobject<UBoxComponent>("Plane Body Collider");
	PlaneBodyBox->SetupAttachment(RootComponent);

	//BackWheelCapsule = CreateDefaultSubobject<UCapsuleComponent>("BackWheelCollider");
	//BackWheelCapsule->SetupAttachment(PlaneBodyBox);

	//FrontLeftWheelCapsule = CreateDefaultSubobject<UCapsuleComponent>("FrontLeftWheelCollider");
	//FrontLeftWheelCapsule->SetupAttachment(PlaneBodyBox);

	//FrontRightWheelCapsule = CreateDefaultSubobject<UCapsuleComponent>("FrontRightWheelCollider");
	//FrontRightWheelCapsule->SetupAttachment(PlaneBodyBox);

	PlaneBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Plane Mesh");
	PlaneBodyMesh->SetupAttachment(PlaneBodyBox);


	PropellerRoot = CreateDefaultSubobject<USceneComponent>("Propeller Root");
	PropellerRoot->SetupAttachment(PlaneBodyMesh);
	PlanePropellerMesh = CreateDefaultSubobject<UStaticMeshComponent>("Plane Propeller Mesh");
	PlanePropellerMesh->SetupAttachment(PropellerRoot);

	AileronLeftRoot = CreateDefaultSubobject<USceneComponent>("Aileron Left Root");
	AileronLeftRoot->SetupAttachment(PlaneBodyMesh);
	AileronLeftMesh = CreateDefaultSubobject<UStaticMeshComponent>("Aileron Left Mesh");
	AileronLeftMesh->SetupAttachment(AileronLeftRoot);

	AileronRightRoot = CreateDefaultSubobject<USceneComponent>("Aileron Right Root");
	AileronRightRoot->SetupAttachment(PlaneBodyMesh);
	AileronRightMesh = CreateDefaultSubobject<UStaticMeshComponent>("Aileron Right Mesh");
	AileronRightMesh->SetupAttachment(AileronRightRoot);

	RudderRoot = CreateDefaultSubobject<USceneComponent>("Rudder Root");
	RudderRoot->SetupAttachment(PlaneBodyMesh);
	RudderMesh = CreateDefaultSubobject<UStaticMeshComponent>("Rudder Mesh");
	RudderMesh->SetupAttachment(RudderRoot);

	ElevatorRoot = CreateDefaultSubobject<USceneComponent>("Elevator Root");
	ElevatorRoot->SetupAttachment(PlaneBodyMesh);
	ElevatorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Elevator Mesh");
	ElevatorMesh->SetupAttachment(ElevatorRoot);

	////CreateMeshWithPivot(AileronRightRoot, AileronRightMesh, "Aileron Right Root", "Aileron Right Mesh");
	//CreateMeshWithPivot(RudderRoot, RudderMesh, "Rudder Root", "Rudder Mesh");
	//CreateMeshWithPivot(ElevatorRoot, ElevatorMesh, "Elevator Root", "Elevator Mesh");

	TailCameraBoom = CreateDefaultSubobject<USpringArmComponent>("Tail Camera Boom");
	TailCameraBoom->SetupAttachment(PlaneBodyMesh);

	TailCamera = CreateDefaultSubobject<UCameraComponent>("Tail Camera");
	TailCamera->SetupAttachment(TailCameraBoom);

	PropellerCameraBoom = CreateDefaultSubobject<USpringArmComponent>("Propeller Camera Boom");
	PropellerCameraBoom->SetupAttachment(PlaneBodyMesh);

	PropellerCamera = CreateDefaultSubobject<UCameraComponent>("Propeller Camera");
	PropellerCamera->SetupAttachment(PropellerCameraBoom);
	
	
	PlanePhysicsComponent = CreateDefaultSubobject<UAircraftPhysics>("Plane Physics");

	
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APlanePawn::SwapCamera()
{
	if (TailCamera->IsActive())
	{
		TailCamera->Deactivate();
		PropellerCamera->Activate();
	}
	else
	{
		TailCamera->Activate();
		PropellerCamera->Deactivate();
	}

}

void APlanePawn::MoveCamera(FVector2D input)
{
	FVector2D difBetweenInputs = input - CameraInput;
	if (difBetweenInputs.Size() < 0.2)
	{
		CameraInput = input;
		return;
	}
	if (abs(input.X) < 0.1)
		input.X = 0;
	if (abs(input.Y) < 0.1)
		input.Y = 0;
	CameraInput = input;
	
	//FVector2D targetAngle = input * FMath::DegreesToRadians(MaxLookAngle);
	FVector2D targetAngle = input * MaxLookAngle;
	TargetCameraRotation = FQuat::MakeFromEuler(FVector(0, targetAngle.Y, targetAngle.X + DefaultCameraRotation.Yaw));

	//if (CameraInput.X > 0)
	//	TargetCameraRotation.X *= -1;


	FQuat currentRot = TailCameraBoom->GetRelativeRotation().Quaternion();

	//FQuat rot = FMath::Lerp(currentRot, targetRot);
	//TailCameraBoom->SetRelativeRotation(targetRot);

	//CameraRotDirection = targetLookAngle - TailCameraBoom->GetRelativeRotation();
	//CameraRotDirection.Normalize();


	//if(abs(inputLookAngle.X) < abs(currentLookAngle.X))
	//	TailCameraBoom->AddLocalRotation(FRotator(0, inputLookAngle.X, 0));
	//if (abs(inputLookAngle.Y) < abs(currentLookAngle.Y))
	//	TailCameraBoom->AddLocalRotation(FRotator(inputLookAngle.Y, 0, 0));
	//TailCameraBoom->AddLocalRotation(FRotator(inputLookAngle.Y, inputLookAngle.X, 0));
}

// Called when the game starts or when spawned
void APlanePawn::BeginPlay()
{
	Super::BeginPlay();
	if (!PlaneBodyBox)
		UE_LOG(LogTemp, Warning, TEXT("Body Null"));
	PlanePhysicsComponent->SetRigidbody(PlaneBodyBox);
	DefaultCameraRotation = TailCameraBoom->GetRelativeRotation();
}

void APlanePawn::CreateMeshWithPivot(USceneComponent* pivot, UStaticMeshComponent* mesh, FName name, FName nameMesh)
{
	pivot = CreateDefaultSubobject<USceneComponent>(name);
	pivot->SetupAttachment(PlaneBodyMesh);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(nameMesh);
	mesh->SetupAttachment(pivot);
}

void APlanePawn::AnimateControlSurface(float input, USceneComponent* surfacePivot, FRotator axis, float dt)
{
	float targetAngle = input * TargetAngle;
	FRotator currentRot = surfacePivot->GetRelativeRotation();
	FVector vector = FVector(currentRot.Roll, currentRot.Pitch, currentRot.Yaw);

	FRotator direction = axis * targetAngle - currentRot;

	direction.Normalize();
	surfacePivot->AddLocalRotation(direction * ControlSurfacesAnimationSpeed * dt);

}

// Called every frame
void APlanePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float rotSpeed = PlanePhysicsComponent->GetThrottle() * PropellerRotationSpeed * DeltaTime;
	PropellerRoot->AddLocalRotation(FRotator(rotSpeed, 0, 0));
	
	
	if(RudderRoot)
		AnimateControlSurface(PlanePhysicsComponent->GetDebugInput().Z, RudderRoot, FRotator(0, 1, 0), DeltaTime);
	if(ElevatorRoot)
		AnimateControlSurface(PlanePhysicsComponent->GetDebugInput().Y, ElevatorRoot, FRotator(0, 0, 1), DeltaTime);
	if (AileronLeftRoot)
		AnimateControlSurface(PlanePhysicsComponent->GetDebugInput().X, AileronLeftRoot, FRotator(0, 0, 1), DeltaTime);
	if (AileronRightRoot)
		AnimateControlSurface(PlanePhysicsComponent->GetDebugInput().X, AileronRightRoot, FRotator(0, 0, 1), DeltaTime);
		//RudderRoot->AddLocalRotation(FRotator(rotSpeed, 0, 0));
			//PlanePhysicsComponent->GetControlInput().Y, PlanePhysicsComponent->GetControlInput().Z);

	//float cameraSpeed = 5.00f * DeltaTime;
	//FRotator targetLookAngle = FRotator(CameraInput.Y * MaxLookAngle.Y, CameraInput.X * MaxLookAngle.X, 0) + DefaultCameraRotation;
	//CameraRotDirection = targetLookAngle - TailCameraBoom->GetRelativeRotation();
	//CameraRotDirection.Normalize();

	//if (CameraInput.SizeSquared() > 0.01)
	//{
	//	FVector rot = TailCameraBoom->GetRelativeRotation().Quaternion().Euler();
	//	if (abs(MaxLookAngle.X) > abs(rot.Y))
	//	{
	//		TailCameraBoom->AddLocalRotation(FRotator(CameraRotDirection.Pitch * cameraSpeed, 0, 0));
	//	}

	//	if (abs(MaxLookAngle.Y) > abs(TailCameraBoom->GetRelativeRotation().Yaw))
	//		TailCameraBoom->AddLocalRotation(FRotator(0, CameraRotDirection.Yaw * cameraSpeed, 0));
	//	//TailCameraBoom->AddLocalRotation(CameraRotDirection * cameraSpeed);
	//	//TailCameraBoom->setlocl

	//	UE_LOG(LogTemp, Warning, TEXT("Pitch: %f"), rot.Y);
	//	UE_LOG(LogTemp, Warning, TEXT("Yaw %f"), rot.Z);
	//}
	//else
	//{
	//	TailCameraBoom->AddLocalRotation(CameraRotDirection * cameraSpeed);
	//}
	rotTimer += DeltaTime;
	rotTimer = FMath::Clamp(rotTimer, 0, 1);
	if (CameraInput.SizeSquared() < 0.01)
	{
		//TailCameraBoom->SetRelativeRotation(DefaultCameraRotation);
		TargetCameraRotation = DefaultCameraRotation.Quaternion();
	}
	else
	{
		int a = 0;
	}
	FQuat currentRot = TailCameraBoom->GetRelativeRotation().Quaternion();
	float speed = CameraMoveSpeed;
	//if (CameraInput.X > 0)
	//	speed *= -1;
	FQuat rot = FMath::Lerp(currentRot, TargetCameraRotation, rotTimer * speed);
	CameraInput = FVector2D(0, 0);
	if ((currentRot - TargetCameraRotation).Size() > 0.01)
		TailCameraBoom->SetRelativeRotation(rot);
	


}

// Called to bind functionality to input
void APlanePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

