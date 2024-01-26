// Fill out your copyright notice in the Description page of Project Settings.
//
#include "PlaneController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlanePawn.h"
#include "Physics/AircraftPhysics.h"

void APlaneController::PreInitializeComponents()
{
	UE_LOG(LogTemp, Warning, TEXT("Pre initialize"));
}

void APlaneController::SetupInputComponent()
{
	// Set up action bindings
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &APlaneController::Throttle);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &APlaneController::Yaw);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &APlaneController::Roll);
		EnhancedInputComponent->BindAction(PitchAction, ETriggerEvent::Triggered, this, &APlaneController::Pitch);
		EnhancedInputComponent->BindAction(CameraSwapAction, ETriggerEvent::Started, this, &APlaneController::SwapCamera);
		EnhancedInputComponent->BindAction(CameraLookAction, ETriggerEvent::Triggered, this, &APlaneController::CameraMovement);
		EnhancedInputComponent->BindAction(CameraLookAction, ETriggerEvent::Started, this, &APlaneController::ResetTimer);
		EnhancedInputComponent->BindAction(LandingGearAction, ETriggerEvent::Started, this, &APlaneController::ToggleWheels);
	}
}

void APlaneController::BeginPlay()
{
	Super::BeginPlay();
	//bShowMouseCursor = true;
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	ControlledPlane = Cast<APlanePawn>(GetPawn());

}

void APlaneController::Yaw(const FInputActionInstance& Instance)
{
	SteeringInput = ControlledPlane->GetPlanePhysicsComponent()->GetControlInput();
	SteeringInput.Z = Instance.GetValue().Get<float>();
	//FloatValue = FMath::Clamp(FloatValue, 0.0f, 1.0f);
	ControlledPlane->GetPlanePhysicsComponent()->UpdateControlInput(SteeringInput);
	UE_LOG(LogTemp, Warning, TEXT("Yaw"));
}

void APlaneController::Roll(const FInputActionInstance& Instance)
{
	SteeringInput = ControlledPlane->GetPlanePhysicsComponent()->GetControlInput();
	SteeringInput.X = Instance.GetValue().Get<float>();
	//FloatValue = FMath::Clamp(FloatValue, 0.0f, 1.0f);
	ControlledPlane->GetPlanePhysicsComponent()->UpdateControlInput(SteeringInput);
}

void APlaneController::Throttle(const FInputActionInstance& Instance)
{
	float FloatValue = Instance.GetValue().Get<float>();
	//FloatValue = FMath::Clamp(FloatValue, 0.0f, 1.0f);
	//ControlledPlane->IncreaseThrust();
	ControlledPlane->GetPlanePhysicsComponent()->SetThrottleInput(FloatValue);
	//ControlledPlane->SetThrust(FloatValue);
	UE_LOG(LogTemp, Warning, TEXT("Throttle Increasing"));
}

void APlaneController::Pitch(const FInputActionInstance& Instance)
{
	SteeringInput = ControlledPlane->GetPlanePhysicsComponent()->GetControlInput();
	SteeringInput.Y = Instance.GetValue().Get<float>();
	//FloatValue = FMath::Clamp(FloatValue, 0.0f, 1.0f);
	ControlledPlane->GetPlanePhysicsComponent()->UpdateControlInput(SteeringInput);

	//UE_LOG(LogTemp, Warning, TEXT("MyValue: %f"), SteeringInput.Y);
}

void APlaneController::SwapCamera()
{
	//ControlledPlane->SwapCamera();
}

void APlaneController::CameraMovement(const FInputActionInstance& Instance)
{
	FVector2D input = Instance.GetValue().Get<FVector2D>();
	//UE_LOG(LogTemp, Warning, TEXT("Look"));
	UE_LOG(LogTemp, Warning, TEXT("Look Input X=%f, Y=%f "), input.X, input.Y);
	ControlledPlane->MoveCamera(input);
}

void APlaneController::ResetTimer()
{
	ControlledPlane->ResetCameraLerpTimer();
}

void APlaneController::ToggleWheels()
{
	ControlledPlane->ToggleLandingGear();
}
