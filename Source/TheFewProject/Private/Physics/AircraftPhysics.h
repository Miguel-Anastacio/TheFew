// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "Components/ActorComponent.h"
#include "AircraftPhysics.generated.h"
USTRUCT(BlueprintType)
struct FBiVector
{
	GENERATED_BODY()

	FVector Force = FVector(0, 0, 0);
	
	FVector Torque = FVector(0, 0, 0);


	FBiVector(FVector f, FVector t) : Force(f), Torque(t) {};
	FBiVector() {};

	FBiVector operator +(FBiVector b)
	{
		return FBiVector(Force + b.Force, this->Torque + b.Torque);
	}
	FBiVector operator *(float b)
	{
		return FBiVector(Force * b, this->Torque * b);
	}

};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UAircraftPhysics : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAircraftPhysics();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetRigidbody(UPrimitiveComponent* rb) { Rigidbody = rb; };

	void SetThrottleInput(float input) { ThrottleInput = input; };
	void UpdateControlInput(FVector input) { ControlInput = input; };
	FVector GetControlInput() { return ControlInput; };
	FVector GetDebugInput() { return InputDebug; };


	float GetThrottle() { return Throttle; };
	float GetMaxThrust() { return MaxThrust; };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FBiVector CalculateAerodynamicsForces(FVector velocity, float angularVelocity, FVector wind, float airDensity, FVector centerOfMass);

	void CalculateState(float deltaTime);
	void CalculateAngleOfAttack();
	void CalculateGForce(float dt);

	FVector CalculateGForceLimit(FVector input);
	FVector EstimateGForce(FVector angularVelocity, FVector velocity);
	float CalculateGLimiter(FVector controlInput, FVector maxAngularVelocity);

	

	void UpdateThrust();
	void UpdateDrag();
	void UpdateLift();

	void UpdateThrottle(float dt);
	void UpdateSteering(float dt);

	FVector CalculateLift(float angleOfAttack, FVector rightAxis, float liftPower, UCurveFloat* aoACurve, UCurveFloat* inducedDragCurve);

	float CalculateSteering(float dt, float angularVelocity, float targetVelocity, float acceleration);

	// sub stepping to remove jitter
	FVector PredictVelocity(FVector force, float dt);
	FVector PredictAngularVelocity(FVector force, float dt);
	FVector Scale6(
		FVector value,
		float posX, float negX,
		float posY, float negY,
		float posZ, float negZ
		);
	float MoveTo(float value, float target, float speed, float deltaTime, float min, float max);

public:	

protected:
	// adjustable plane settings
	UPROPERTY(EditAnywhere, Category = "Plane Settings")
		float MaxThrust = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Plane Settings")
		float ThrottleSpeed = 0;
	UPROPERTY(EditAnywhere, Category = "Plane Settings")
		float BreakSpeed = 0;
	// adjustable plane settings
	UPROPERTY(EditAnywhere, Category = "Plane Settings")
		float Scale = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "Drag")
		UCurveFloat* DragForward;
	UPROPERTY(EditAnywhere, Category = "Drag")
		UCurveFloat* DragBack;
	UPROPERTY(EditAnywhere, Category = "Drag")
		UCurveFloat* DragLeft;
	UPROPERTY(EditAnywhere, Category = "Drag")
		UCurveFloat* DragRight;
	UPROPERTY(EditAnywhere, Category = "Drag")
		UCurveFloat* DragTop;
	UPROPERTY(EditAnywhere, Category = "Drag")
		UCurveFloat* DragBottom;

	UPROPERTY(EditAnywhere, Category = "Lift")
		UCurveFloat* AoACurve;
	UPROPERTY(EditAnywhere, Category = "Lift")
		UCurveFloat* InducedDragCurve;
	UPROPERTY(EditAnywhere, Category = "Lift")
		float LiftPower = 150;
	UPROPERTY(EditAnywhere, Category = "Lift")
		float InducedDrag = 150;
	UPROPERTY(EditAnywhere, Category = "Lift")
		float RudderPower = 100;
	UPROPERTY(EditAnywhere, Category = "Lift")
		UCurveFloat* RudderAoACurve;

	// controls turn speed on each axis in degree per second
	UPROPERTY(EditAnywhere, Category = "Steering")
		FVector TurnSpeed = FVector(180, 30, 15);
	UPROPERTY(EditAnywhere, Category = "Steering")
		FVector TurnAcceleration = FVector(360, 60, 30);
	UPROPERTY(EditAnywhere, Category = "Steering")
		UCurveFloat* SteeringCurve;

	UPROPERTY(EditAnywhere, Category = "GForce")
		float GLimit = 8.0f;
	UPROPERTY(EditAnywhere, Category = "GForce")
		float GLimitPitchDown = -4.0f;

	UPROPERTY(Transient)
		UPrimitiveComponent* Rigidbody;

	float ThrustPercent = 0;
	// debug variables
	FVector CurrentForce;
	FVector CurrentDrag;
	FVector CurrentLift;
	FVector CurrentInducedDrag;

	FVector Velocity;
	FVector LastVelocity;

	FVector LocalVelocity;
	FVector LocalAngularVelocity;
	FVector LocalGForce;

	float AngleOfAttack = 0;
	float AngleOfAttackYaw = 0;

	// input variables
	float Throttle = 0;
	float ThrottleInput = 1;
	FVector ControlInput = FVector(0, 0, 0);

	// SteeringData
	float SteeringPower = 0.f;
	float GForceScaling = 0.f;
	FVector GForceLimitDebug;
	FVector MaxGForceDebug;
	FVector TargetAngularVelocity;
	FVector CurrentAngularVelocity;
	FVector TorqueApplied;
	FVector InputDebug;

	friend class APlanePhysicsDebugHUD;
};
