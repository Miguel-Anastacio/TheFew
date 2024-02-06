// Fill out your copyright notice in the Description page of Project Settings.

#include "PlanePawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Physics/AircraftPhysics.h"
#include "Camera/CameraComponent.h"
#include "Weapon/WeaponComponent.h"
#include "VFX/VfxComponent.h"
#include "Components/AudioComponent.h"
// Sets default values
APlanePawn::APlanePawn()
{
	//PlaneRoot = CreateDefaultSubobject<USceneComponent>("Root");
	
	PlaneBodyBox = CreateDefaultSubobject<UBoxComponent>("Plane Body Collider");
	//PlaneBodyBox->SetupAttachment(RootComponent);
	RootComponent = PlaneBodyBox;

	BackWheel = CreateDefaultSubobject<UCapsuleComponent>("Back Wheel");
	BackWheel->SetupAttachment(PlaneBodyBox);
	//BackWheelCapsule->bAutoActivate = fflae;


	LeftWheelCollider = CreateDefaultSubobject<UCapsuleComponent>("Left Wheel Collider");
	LeftWheelCollider->SetupAttachment(PlaneBodyBox);
	//FrontLeftWheelCapsule->bAutoActivate = false;

	RightWheelCollider = CreateDefaultSubobject<UCapsuleComponent>("Right Wheel Collider");
	RightWheelCollider->SetupAttachment(PlaneBodyBox);
	//FrontRightWheelCapsule->bAutoActivate = false;

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

	LeftLandingGearRoot = CreateDefaultSubobject<USceneComponent>("Landing Gear Left Root");
	LeftLandingGearRoot->SetupAttachment(PlaneBodyMesh);
	LeftLandingGearMesh = CreateDefaultSubobject<UStaticMeshComponent>("Landing Gear Left Mesh");
	LeftLandingGearMesh->SetupAttachment(LeftLandingGearRoot);

	RightLandingGearRoot = CreateDefaultSubobject<USceneComponent>("Landing Gear Right Root");
	RightLandingGearRoot->SetupAttachment(PlaneBodyMesh);
	RightLandingGearMesh = CreateDefaultSubobject<UStaticMeshComponent>("Landing Gear Right Mesh");
	RightLandingGearMesh->SetupAttachment(RightLandingGearRoot);
	
	
	//CreateMeshWithPivot(LeftLandingGearRoot, LeftLandingGearMesh, "Landing Gear Left Root", "Landing Gear Left Mesh");
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

	LeftWeaponComponent = CreateDefaultSubobject<UWeaponComponent>("Left Weapon");
	LeftWeaponComponent->SetupAttachment(PlaneBodyBox);
	RightWeaponComponent = CreateDefaultSubobject<UWeaponComponent>("Right Weapon");
	RightWeaponComponent->SetupAttachment(PlaneBodyBox);

	LeftTrail = CreateDefaultSubobject<UVfxComponent>("Left Trail");
	LeftTrail->SetupAttachment(PlaneBodyBox);
	RightTrail = CreateDefaultSubobject<UVfxComponent>("Right Trail");
	RightTrail->SetupAttachment(PlaneBodyBox);
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	PlaneEngineAudioComponent = CreateDefaultSubobject<UAudioComponent>("Engine Audio");
	GunFireAudioComponent = CreateDefaultSubobject<UAudioComponent>("Gun Audio");
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
	/*FVector2D difBetweenInputs = input - CameraInput;
	if (difBetweenInputs.Size() < 0.2)
	{
		CameraInput = input;
		return;
	}*/

	if (abs(input.X) < 0.1)
		input.X = 0;
	if (abs(input.Y) < 0.1)
		input.Y = 0;
	CameraInput = input;
	
	////FVector2D targetAngle = input * FMath::DegreesToRadians(MaxLookAngle);
	//FVector2D targetAngle = input * MaxLookAngle;
	//TargetCameraRotation = FQuat::MakeFromEuler(FVector(0, targetAngle.Y, targetAngle.X + DefaultCameraRotation.Yaw));

	////if (CameraInput.X > 0)
	////	TargetCameraRotation.X *= -1;


	//FQuat currentRot = TailCameraBoom->GetRelativeRotation().Quaternion();

	////FQuat rot = FMath::Lerp(currentRot, targetRot);
	//TailCameraBoom->SetRelativeRotation(targetRot);

	//CameraRotDirection = targetLookAngle - TailCameraBoom->GetRelativeRotation();
	//CameraRotDirection.Normalize();


	//if(abs(inputLookAngle.X) < abs(currentLookAngle.X))
	//	TailCameraBoom->AddLocalRotation(FRotator(0, inputLookAngle.X, 0));
	//if (abs(inputLookAngle.Y) < abs(currentLookAngle.Y))
	//	TailCameraBoom->AddLocalRotation(FRotator(inputLookAngle.Y, 0, 0));
	//TailCameraBoom->AddLocalRotation(FRotator(inputLookAngle.Y, inputLookAngle.X, 0));

}

void APlanePawn::UpdateCamera(float DeltaTime)
{
	// update fov
	float currentSpeed = PlaneBodyBox->GetPhysicsLinearVelocity().Size();
	currentSpeed = FMath::Clamp(currentSpeed, 0, SpeedForMaxFOV);
	float fov = FMath::InterpEaseIn(DefaultFOV, MaxFOV, currentSpeed / SpeedForMaxFOV, 5);

	TailCamera->FieldOfView = fov;
	
	//// update camera position
	//rotTimer += DeltaTime;
	//rotTimer = FMath::Clamp(rotTimer, 0, 1);
	if (CameraInput.SizeSquared() < 0.01)
	{
		//TailCameraBoom->SetRelativeRotation(DefaultCameraRotation);
		TargetCameraRotation = DefaultCameraRotation.Quaternion();
		
		TailCameraBoom->SetRelativeRotation(FMath::Lerp(TailCameraBoom->GetRelativeRotation(), DefaultCameraRotation, DeltaTime));
		return;
	}
	//FQuat currentRot = TailCameraBoom->GetRelativeRotation().Quaternion();
	//float speed = CameraMoveSpeed;

	//FQuat rot = FMath::Lerp(currentRot, TargetCameraRotation, rotTimer * speed);
	//CameraInput = FVector2D(0, 0);

	//if ((currentRot - TargetCameraRotation).Size() > 0.01)
	//	TailCameraBoom->SetRelativeRotation(rot);
	
	FRotator rot = TailCameraBoom->GetRelativeRotation();
	/*if (!FMath::IsNearlyEqual(abs(rot.Pitch + CameraInput.Y * CameraMoveSpeed), abs(MaxLookAngle.X), 0.1))
	{
		rot.Pitch += CameraInput.Y * CameraMoveSpeed;
	}
	if (abs(rot.Yaw + CameraInput.X * CameraMoveSpeed) - abs(MaxLookAngle.Y *(-CameraInput.X) + DefaultCameraRotation.Yaw) < 0)
	{
		rot.Yaw += CameraInput.X * CameraMoveSpeed;
	}
	else
	{
		return;
	}*/
	FVector2D YawLimits = FVector2D(DefaultCameraRotation.Yaw - MaxLookAngle.Y, DefaultCameraRotation.Yaw + MaxLookAngle.Y);
	if (CameraInput.X < 0 && abs(rot.Yaw + CameraInput.X * CameraMoveSpeed * DeltaTime) > YawLimits.X)
	{
		rot.Yaw += CameraInput.X * CameraMoveSpeed * DeltaTime;
	}
	else if (CameraInput.X > 0 && abs(rot.Yaw + CameraInput.X * CameraMoveSpeed * DeltaTime) < YawLimits.Y)
	{
		rot.Yaw += CameraInput.X * CameraMoveSpeed * DeltaTime;
	}

	FVector2D PitchLimits = FVector2D(DefaultCameraRotation.Pitch + MaxLookAngle.X, DefaultCameraRotation.Pitch - MaxLookAngle.X);
	if (CameraInput.Y < 0 && abs(rot.Pitch + CameraInput.Y * CameraMoveSpeed * DeltaTime) < abs(PitchLimits.Y))
	{
		rot.Pitch += CameraInput.Y * CameraMoveSpeed * DeltaTime;
	}
	else if (CameraInput.Y > 0 && rot.Pitch + CameraInput.Y * CameraMoveSpeed * DeltaTime < PitchLimits.X)
	{
		rot.Pitch += CameraInput.Y * CameraMoveSpeed * DeltaTime;
	}


	//rot.Yaw += CameraInput.X * CameraMoveSpeed;
	TailCameraBoom->SetRelativeRotation(rot);
	//TailCameraBoom->SetRelativeRotation(rot);


	CameraInput = FVector2D(0, 0);
}

void APlanePawn::ToggleLandingGear()
{
	LandingGear = !LandingGear;
	
	if (LandingGear)
	{
		LeftLandingGearRoot->SetRelativeRotation(FRotator(90, 0, 0));
		RightLandingGearRoot->SetRelativeRotation(FRotator(-90, 0, 0));
		//FrontRightWheelCapsule->Activakte();

		LeftWheelCollider->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		RightWheelCollider->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		BackWheel->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else
	{
		//FrontRightWheelCapsule->Deactivate();
		LeftWheelCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RightWheelCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BackWheel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//FrontLeftWheelCapsule->Deactivate();
		LeftLandingGearRoot->SetRelativeRotation(FRotator(0, 0, 0));
		RightLandingGearRoot->SetRelativeRotation(FRotator(0, 0, 0));
	}
}

UPrimitiveComponent* APlanePawn::GetRigidbody()
{
	return PlanePhysicsComponent->GetRigidbody();
}

void APlanePawn::TriggerWeapons()
{
	if(!GunFireAudioComponent->IsPlaying())
		GunFireAudioComponent->Play();
	LeftWeaponComponent->FireBullet();
	RightWeaponComponent->FireBullet();
}

void APlanePawn::StopWeaponAudio()
{
	if (GunFireAudioComponent->IsPlaying())
		GunFireAudioComponent->Stop();
}

// Called when the game starts or when spawned
void APlanePawn::BeginPlay()
{
	Super::BeginPlay();
	if (!PlaneBodyBox)
		UE_LOG(LogTemp, Warning, TEXT("Body Null"));
	PlanePhysicsComponent->SetRigidbody(PlaneBodyBox);

	DefaultCameraRotation = TailCameraBoom->GetRelativeRotation();
	
	DefaultFOV = TailCamera->FieldOfView;
	Flying = true;
	if (!LandingGear)
	{
		LeftWheelCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RightWheelCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BackWheel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}


	LeftTrail->SetSpeedForMaxTrail(SpeedForMaxFOV);
	RightTrail->SetSpeedForMaxTrail(SpeedForMaxFOV);

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

void APlanePawn::UpdateFlying()
{
	FHitResult Hit;
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = GetActorLocation() + FVector::DownVector * 200.0f;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldStatic, QueryParams);
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.1f, 0, 1.0f);
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		if (Flying)
		{
			FVector current = PlaneBodyBox->GetPhysicsAngularVelocityInDegrees();
			PlaneBodyBox->SetPhysicsAngularVelocityInDegrees(FVector(current.X, current.Y, 0));
		}

		Flying = false;
		PlanePhysicsComponent->Setflaps(true);
	}
	else 
	{
		PlanePhysicsComponent->Setflaps(false);
		Flying = true;
	}
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

	UpdateCamera(DeltaTime);
	
	//LeftWeaponComponent->SetActorForwardVector(PlaneBodyBox->GetForwardVector());
	//RightWeaponComponent->SetActorForwardVector(PlaneBodyBox->GetForwardVector());

	UpdateFlying();

	if (Flying)
	{
		LeftTrail->UpdateNiagaraSystem(GetVelocity());
		RightTrail->UpdateNiagaraSystem(GetVelocity());
	}

	if (!PlaneEngineAudioComponent)
	{
		return;
	}

	if (!PlaneEngineAudioComponent->IsPlaying())
	{
		PlaneEngineAudioComponent->Play();
	}

	PlaneEngineAudioComponent->SetVolumeMultiplier(PlanePhysicsComponent->GetThrottle());
}

// Called to bind functionality to input
void APlanePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

