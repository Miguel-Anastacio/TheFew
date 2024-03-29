// Fill out your copyright notice in the Description page of Project Settings.
//
#include "PlaneController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlanePawn.h"
#include "PlanePawnAI.h"
#include "Player/PlanePawnPlayer.h"
#include "Physics/AircraftPhysics.h"
#include "UI/PlaneHUD.h"
#include "Weapon/WeaponComponent.h"
#include "Managers/AIManager.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/ReactToClickInterface.h"
#include "BattlePlaneGameMode.h"
#include "Components/HealthComponent.h"
#include "UI/Manager/ManagerHUD.h"
void APlaneController::InitDebugVariables(AActor* landscape, AAIManager* manager)
{
	LandscapeActor = landscape;
	AIManager = manager;

	if (AIManager)
	{
		AIManager->TeamDataInitDelegate.AddDynamic(HUDManager, &AManagerHUD::DisplaySpawnScreen);
	}
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

		// Debug Actions
		EnhancedInputComponent->BindAction(ChangePlaneSelectedAction, ETriggerEvent::Started, this, &APlaneController::ChangeFocusedPlane);
		EnhancedInputComponent->BindAction(ChangeToLevelAction, ETriggerEvent::Started, this, &APlaneController::FocusOnLevel);

		// UI Actions
		EnhancedInputComponent->BindAction(ToggleScoreboardAction, ETriggerEvent::Started, this, &APlaneController::ToggleScoreboard);

		// Menu actions
		EnhancedInputComponent->BindAction(MouseClickAction, ETriggerEvent::Started, this, &APlaneController::MouseClick);
		EnhancedInputComponent->BindAction(ControllerInputUIAction, ETriggerEvent::Started, this, &APlaneController::ControllerInputUI);
		EnhancedInputComponent->BindAction(ControllerCursorAction, ETriggerEvent::Triggered, this, &APlaneController::AnalogStickMovement);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &APlaneController::TogglePause);
	}
}

void APlaneController::BeginPlay()
{
	Super::BeginPlay();
	//bShowMouseCursor = true;
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		//Subsystem->AddMappingContext(DefaultMappingContext, 0);
		Subsystem->AddMappingContext(UIMappingContext, 1);
	}


	ABattlePlaneGameMode* gameMode = Cast< ABattlePlaneGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(gameMode))
	{
		gameMode->PlayingStateDelegate.AddDynamic(this, &APlaneController::TransitionSpawnToPlaying);
		gameMode->SpawnMenuStateDelegate.AddDynamic(this, &APlaneController::FocusOnMap);
		gameMode->EndOfRoundStateDelegate.AddDynamic(this, &APlaneController::TransitionToEndOFRound);
	}

	if (IsValid(LandscapeActor))
		this->SetViewTarget(LandscapeActor);

	SetShowMouseCursor(true);
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);


}

void APlaneController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	ControlledPlane = Cast<APlanePawnPlayer>(pawn);
	ControlledPlane->GetHealthComponent()->ActorDeathDelegate.AddDynamic(this, &APlaneController::OnPlayerDeath);


	HUDManager = Cast<AManagerHUD>(GetHUD());
	if(HUDManager)
		HUDManager->Init();
	
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

void APlaneController::ToggleScoreboard()
{
	if (HUDManager)
	{
		HUDManager->ToggleScoreboard(false);
	}
}

void APlaneController::TogglePause()
{
	SetPause(!IsPaused());
	bShowMouseCursor = IsPaused();
	if (bShowMouseCursor)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(UIMappingContext, 1);
			//Subsystem->RemoveMappingContext(UIMappingContext);
		}

		FInputModeGameAndUI InputMode;
		SetInputMode(InputMode);
	}
	else
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
		{
			//Subsystem->AddMappingContext(UIMappingContext, 1);
			Subsystem->RemoveMappingContext(UIMappingContext);
		}
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
	}
	HUDManager->TogglePause();
}

void APlaneController::MouseClick()
{
	FHitResult hit;
	ETraceTypeQuery traceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);

	GetHitResultUnderCursorByChannel(traceType, false, hit);

	AActor* actor = hit.GetActor();

	if (!IsValid(actor))
		return;
	IReactToClickInterface* interface = Cast<IReactToClickInterface>(actor);

	if (interface)
	{
		interface->ReactToClick();
	}
}

void APlaneController::ControllerInputUI()
{
	bShowMouseCursor = false;
	//PlaneHUD->FocusActiveWidget();
	HUDManager->FocusActiveWidget();
}

void APlaneController::AnalogStickMovement(const FInputActionInstance& Instance)
{
	bShowMouseCursor = true;
	HUDManager->UnFocusActiveWidget();
	FVector2D input = Instance.GetValue().Get<FVector2D>();
	input.Y *= -1;
	FVector2D mousePos;
	GetMousePosition(mousePos.X, mousePos.Y);
	mousePos += input;
	SetMouseLocation(mousePos.X, mousePos.Y);
}

void APlaneController::TransitionSpawnToPlaying(const FVector& location)
{
	if (IsValid(ControlledPlane))
	{
		//PlaneHUD->RemoveSpawnScreen();
		// remove spawn 
		HUDManager->PopFromLayerWithFocus(HUDManager->Priority);
		// add hud
		HUDManager->DisplayHUD();
		
		ControlledPlane->RespawnPlayer(location);

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			//Subsystem->AddMappingContext(UIMappingContext, 1);
			Subsystem->RemoveMappingContext(UIMappingContext);
		}

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(false);
		this->SetViewTargetWithBlend(ControlledPlane, TimeSpawnToPlayCameraTransition);
	}
}

void APlaneController::FocusOnMap()
{
	if (LandscapeActor)
	{
		this->SetViewTargetWithBlend(LandscapeActor, TimeDeathToSpawnCameraTransition);
	}
	SetShowMouseCursor(true);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		//Subsystem->AddMappingContext(DefaultMappingContext, 0);
		Subsystem->RemoveMappingContext(DefaultMappingContext);
		Subsystem->AddMappingContext(UIMappingContext, 1);
	}
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	//Subsystem->AddMappingContext(UIMappingContext, 1);
}

void APlaneController::TransitionToEndOFRound()
{
	FocusOnMap();
	//Pause();
	ControlledPlane->PlaneDeathSimple();

}

void APlaneController::OnPlayerDeath(AActor* other)
{
	StopFiring();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		//Subsystem->AddMappingContext(DefaultMappingContext, 0);
		Subsystem->RemoveMappingContext(DefaultMappingContext);
		Subsystem->AddMappingContext(UIMappingContext, 1);
	}
}

// Debug Action FUNCTION
void APlaneController::ChangeFocusedPlane(const FInputActionInstance& Instance)
{
	float input = Instance.GetValue().Get<float>();
	if (IsValid(AIManager))
	{
		CurrentAISelected = AIManager->ChangePlaneSelected(input);
		this->SetViewTarget(CurrentAISelected);
	}

}

// Debug Action FUNCTION
void APlaneController::FocusOnLevel()
{
	if (FocusingOnLevel)
	{
		FocusingOnLevel = false;
		if(IsValid(CurrentAISelected))
			this->SetViewTarget(CurrentAISelected);
	}
	else
	{
		FocusingOnLevel = true;
		if(IsValid(LandscapeActor))
			this->SetViewTarget(LandscapeActor);
	}
}
