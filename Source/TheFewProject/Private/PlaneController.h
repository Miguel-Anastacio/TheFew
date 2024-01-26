// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlaneController.generated.h"

class UInputMappingContext;
class UInputAction;
class APlanePawn;

struct FInputActionInstance;
/**
 * 
 */
UCLASS()
class APlaneController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	void PreInitializeComponents() override;
	void SetupInputComponent() override;
	void BeginPlay() override;
	// Input Fucntions
	void Yaw(const FInputActionInstance& Instance);
	void Roll(const FInputActionInstance& Instance);
	void Throttle(const FInputActionInstance& Instance);
	void Pitch(const FInputActionInstance& Instance);
	void SwapCamera();
	void CameraMovement(const FInputActionInstance& Instance);
	void ResetTimer();
	void ToggleWheels();
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

	APlanePawn* ControlledPlane;

	FVector SteeringInput;
};
