// Fill out your copyright notice in the Description page of Project Settings.
//
#include "PlaneController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlanePawn.h"
#include "PlanePawnAI.h"
#include "Physics/AircraftPhysics.h"
#include "UI/PlaneHUD.h"
#include "Weapon/WeaponComponent.h"
#include "Managers/AIManager.h"
#include "Kismet/GameplayStatics.h"
void APlaneController::InitDebugVariables(AActor* landscape, AAIManager* manager)
{
	LandscapeActor = landscape;
	AIManager = manager;
}
APlanePawn* APlaneController::GetPlaneSelected()
{
	return CurrentAISelected;
}
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
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlaneController::Fire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Canceled, this, &APlaneController::StopFiring);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &APlaneController::StopFiring);

		EnhancedInputComponent->BindAction(ChangePlaneSelectedAction, ETriggerEvent::Started, this, &APlaneController::ChangeFocusedPlane);
		EnhancedInputComponent->BindAction(ChangeToLevelAction, ETriggerEvent::Started, this, &APlaneController::FocusOnLevel);
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

	//ControlledPlane = Cast<APlanePawn>(GetPawn());

	//PlaneHUD = CreateWidget<UPlaneHUD>(this, PlaneHUDClass);
	//PlaneHUD->AddToViewport();
	//PlaneHUD->SetPlaneReference(ControlledPlane);

}

void APlaneController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	ControlledPlane = Cast<APlanePawn>(pawn);

	PlaneHUD = CreateWidget<UPlaneHUD>(this, PlaneHUDClass);
	PlaneHUD->AddToViewport();
	PlaneHUD->SetPlaneReference(ControlledPlane);
}

void APlaneController::Yaw(const FInputActionInstance& Instance)
{
	SteeringInput = ControlledPlane->GetPlanePhysicsComponent()->GetControlInput();
	SteeringInput.Z = Instance.GetValue().Get<float>();
	//FloatValue = FMath::Clamp(FloatValue, 0.0f, 1.0f);
	ControlledPlane->GetPlanePhysicsComponent()->UpdateControlInput(SteeringInput);
	//UE_LOG(LogTemp, Warning, TEXT("Yaw"));
}

void APlaneController::Roll(const FInputActionInstance& Instance)
{
	SteeringInput = ControlledPlane->GetPlanePhysicsComponent()->GetControlInput();
	SteeringInput.X = Instance.GetValue().Get<float>();

	if (abs(SteeringInput.X) < MovementDeadzone.X)
		SteeringInput.X = 0;
	//FloatValue = FMath::Clamp(FloatValue, 0.0f, 1.0f);
	if (!ControlledPlane->GetIsFlying())
		SteeringInput.X *= 0.01;
	ControlledPlane->GetPlanePhysicsComponent()->UpdateControlInput(SteeringInput);
}

void APlaneController::Throttle(const FInputActionInstance& Instance)
{
	float FloatValue = Instance.GetValue().Get<float>();
	//FloatValue = FMath::Clamp(FloatValue, 0.0f, 1.0f);
	//ControlledPlane->IncreaseThrust();
	ControlledPlane->GetPlanePhysicsComponent()->SetThrottleInput(FloatValue);
	//ControlledPlane->SetThrust(FloatValue);
	//UE_LOG(LogTemp, Warning, TEXT("Throttle Increasing"));
}

void APlaneController::Pitch(const FInputActionInstance& Instance)
{
	SteeringInput = ControlledPlane->GetPlanePhysicsComponent()->GetControlInput();
	SteeringInput.Y = Instance.GetValue().Get<float>();

	if (abs(SteeringInput.Y) < MovementDeadzone.Y)
		SteeringInput.Y = 0;

	//FloatValue = FMath::Clamp(FloatValue, 0.0f, 1.0f);
	ControlledPlane->GetPlanePhysicsComponent()->UpdateControlInput(SteeringInput);

	//UE_LOG(LogTemp, Warning, TEXT("MyValue: %f"), SteeringInput.Y);
}

void APlaneController::SwapCamera()
{
	ControlledPlane->SwapCamera();
}

void APlaneController::CameraMovement(const FInputActionInstance& Instance)
{
	FVector2D input = Instance.GetValue().Get<FVector2D>();
	//UE_LOG(LogTemp, Warning, TEXT("Look"));
	//UE_LOG(LogTemp, Warning, TEXT("Look Input X=%f, Y=%f "), input.X, input.Y);
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

void APlaneController::Fire()
{
	//UE_LOG(LogTemp, Warning, TEXT("Fire"));
	ControlledPlane->TriggerWeapons();
}

void APlaneController::StopFiring()
{
	ControlledPlane->StopWeaponAudio();
}

void APlaneController::ChangeFocusedPlane(const FInputActionInstance& Instance)
{
	float input = Instance.GetValue().Get<float>();
	if (IsValid(AIManager))
	{
		CurrentAISelected = AIManager->ChangePlaneSelected(input);
		this->SetViewTarget(CurrentAISelected);
	}

}

void APlaneController::FocusOnLevel()
{
	if (FocusingOnLevel)
	{
		FocusingOnLevel = false;	
		this->SetViewTarget(CurrentAISelected);
	}
	else
	{
		FocusingOnLevel = true;
		this->SetViewTarget(LandscapeActor);
	}
}
