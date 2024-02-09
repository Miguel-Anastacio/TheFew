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
protected:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* DefaultMappingContext;
	/** Mouse Input Action */
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
		FVector2D MovementDeadzone = FVector2D(0.1, 0.1);

	// UI
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UPlaneHUD> PlaneHUDClass;

	APlanePawn* ControlledPlane;

	UPlaneHUD* PlaneHUD;

	FVector SteeringInput;




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

	TObjectPtr<class APlanePawnAI> CurrentAISelected;

	void ChangeFocusedPlane(const FInputActionInstance& Instance);
	void FocusOnLevel();

	bool FocusingOnLevel = false;


};
