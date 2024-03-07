// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlaneController.generated.h"

class UInputMappingContext;
class UInputAction;
class APlanePawn;
class UPlaneHUD;

struct FInputActionInstance;
/**
 * 
 */
UCLASS()
class APlaneController : public APlayerController
{
	GENERATED_BODY()

public:
	void InitDebugVariables(AActor* landscape, class AAIManager* manager);
	APlanePawn* GetPlaneSelected();
	//void SetPlaneSelected(APlanePawn* plane) { CurrentAISelected = plane; };
	AAIManager* GetAIManager() {
		return AIManager;
	};

	UPlaneHUD* GetWidgetHUD() { return PlaneHUD; };

protected:
	void PreInitializeComponents() override;
	void SetupInputComponent() override;
	void BeginPlay() override;
	void OnPossess(APawn* pawn) override;
	// Input Fucntions
	void Yaw(const FInputActionInstance& Instance);
	void Roll(const FInputActionInstance& Instance);
	void Throttle(const FInputActionInstance& Instance);
	void Pitch(const FInputActionInstance& Instance);
	void SwapCamera();
	void CameraMovement(const FInputActionInstance& Instance);
	void ResetTimer();
	void ToggleWheels();
	void Fire();
	void StopFiring();
	void ToggleScoreboard();

	UFUNCTION(BlueprintCallable)
	void TogglePause();

	void MouseClick();

	// functions bound to game mode state transitions
	UFUNCTION()
	void TransitionSpawnToPlaying(const FVector& location);
	UFUNCTION()
		void FocusOnMap();
	UFUNCTION()
		void TransitionToEndOFRound();
	UFUNCTION()
		void OnPlayerDeath(AActor* other);
	
	// MoveToCreatePlugin()
	void ControllerInputUI();
	void AnalogStickMovement(const FInputActionInstance& Instance);
protected:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* ThrottleAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* RollAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* TurnAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* PitchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* CameraSwapAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* CameraLookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* LandingGearAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* ToggleScoreboardAction;

	// UI Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* UIMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* MouseClickAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		FVector2D MovementDeadzone = FVector2D(0.1, 0.1);

	// Move to create plugin
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ControllerInputUIAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ControllerCursorAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ControllerSelectAction;
	UPROPERTY(EditAnywhere, Category = " Transitions")
		float TimeSpawnToPlayCameraTransition = 2.0f;
	UPROPERTY(EditAnywhere, Category = " Transitions")
		float TimeDeathToSpawnCameraTransition = 1.0f;
	// UI
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> PlaneHUDClass;
	UPROPERTY(BlueprintReadWrite)
	UPlaneHUD* PlaneHUD;

	TObjectPtr<class APlanePawnPlayer> ControlledPlane;

	FVector SteeringInput;

	bool ScoreboardStatus = false;


	/*
	*		DEBUG AI STUFF 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* ChangePlaneSelectedAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* ChangeToLevelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<AActor> LandscapeActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class AAIManager> AIManager;

	TObjectPtr<class APlanePawn> CurrentAISelected;

	void ChangeFocusedPlane(const FInputActionInstance& Instance);
	void FocusOnLevel();

	bool FocusingOnLevel = false;


};
