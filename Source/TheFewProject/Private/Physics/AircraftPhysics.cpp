// Fill out your copyright notice in the Description page of Project Settings.

#include "Physics/AircraftPhysics.h"
#include "../../TheFewProject.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UAircraftPhysics::UAircraftPhysics()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UAircraftPhysics::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogProjectFew, Log, TEXT("Hello Custom Log"));
	// ...
	
}

FBiVector UAircraftPhysics::CalculateAerodynamicsForces(FVector velocity, float angularVelocity, FVector wind, float airDensity, FVector centerOfMass)
{
	FBiVector forceAndTorque;

	return FBiVector();
}

void UAircraftPhysics::CalculateState(float deltaTime)
{
	FQuat invRotation = Rigidbody->GetRelativeRotation().GetInverse().Quaternion();
	Velocity = Rigidbody->GetPhysicsLinearVelocity();


	FTransform localTransform = Rigidbody->GetComponentTransform().Inverse();
	LocalVelocity = localTransform.TransformVector(Velocity);


	//LocalVelocity = invRotation * Velocity;
	LocalAngularVelocity = localTransform.TransformVector(Rigidbody->GetPhysicsAngularVelocityInDegrees());
	LocalAngularVelocity = invRotation.RotateVector(Rigidbody->GetPhysicsAngularVelocityInDegrees());
	//LocalAngularVelocity = Rigidbody->GetPhysicsAngularVelocityInDegrees();

	CalculateAngleOfAttack();

}

void UAircraftPhysics::CalculateAngleOfAttack()
{
	if (LocalVelocity.SizeSquared() < 0.1f)
	{
		AngleOfAttack = 0;
		AngleOfAttackYaw = 0;
		return;
	}

	FVector forwardVector = Rigidbody->GetForwardVector();
	FVector relativeWind = Velocity - forwardVector;

	AngleOfAttack = FMath::Atan2(-LocalVelocity.Z, LocalVelocity.X);
	AngleOfAttackYaw = FMath::Atan2(LocalVelocity.Y, LocalVelocity.X);

	AngleOfAttack = FMath::Acos(FVector::DotProduct(forwardVector.GetSafeNormal(), relativeWind.GetSafeNormal()));

	//// Check if the aircraft is upside down
	//if (forwardVector.Z < 0) {
	//	AngleOfAttack = 2 * PI - AngleOfAttack;
	//}
}

void UAircraftPhysics::CalculateGForce(float dt)
{
	FQuat invRotation = Rigidbody->GetRelativeRotation().GetInverse().Quaternion();
	FVector acceleration = (Velocity - LastVelocity) / dt;

	LocalGForce = invRotation * acceleration;
	LastVelocity = Velocity;

}

FVector UAircraftPhysics::CalculateGForceLimit(FVector input)
{
	return Scale6(input,
		GLimit, GLimit,
		GLimitPitchDown, GLimit,
		GLimit, GLimit) * 9.8f * Scale;

}

FVector UAircraftPhysics::EstimateGForce(FVector angularVelocity, FVector velocity)
{
	//estiamte G Force from angular velocity and velocity
	   //Velocity = AngularVelocity * Radius
	   //G = Velocity^2 / R
	   //G = (Velocity * AngularVelocity * Radius) / Radius
	   //G = Velocity * AngularVelocity
	   //G = V cross A
	return FVector::CrossProduct(angularVelocity, velocity);
}

float UAircraftPhysics::CalculateGLimiter(FVector controlInput, FVector maxAngularVelocity)
{
	controlInput.Normalize();
	FVector limit = CalculateGForceLimit(controlInput);
	GForceLimitDebug = limit;
	FVector maxGForce = EstimateGForce(controlInput * maxAngularVelocity, LocalVelocity);
	MaxGForceDebug = maxGForce;
	if (maxGForce.SizeSquared() > limit.SizeSquared()) {
		//example:
		//maxGForce = 16G, limit = 8G
		//so this is 8 / 16 or 0.5
		return limit.Size() / maxGForce.Size();
	}

	return 1.0f;
}

void UAircraftPhysics::UpdateThrust()
{
	FVector thrust = FVector(1, 0, 0) * Throttle * MaxThrust * Scale;

	//FVector GravityForce = FVector(0.0f, 0.0f, -GetWorld()->GetGravityZ() * Rigidbody->GetMass());
	FTransform worldTransform = Rigidbody->GetComponentTransform();
	thrust = worldTransform.TransformVector(thrust);
	Rigidbody->AddForce(thrust);
	CurrentForce = thrust;
	
}

void UAircraftPhysics::UpdateDrag()
{
	FVector lv = LocalVelocity;
	float lv2 = lv.SizeSquared();

	// adjust based on scale to interact with curves
	lv /= Scale;
	//UE_LOG(LogTemp, Warning, TEXT("LocalVelocityDrag: X=%f, Y=%f, Z=%f"), lv.X, lv.Y, lv.Z);
	FVector dragCoefficient = Scale6(
		lv.GetSafeNormal(),
		DragForward->GetFloatValue(abs(lv.X)), DragBack->GetFloatValue(abs(lv.X)), 
		DragRight->GetFloatValue(abs(lv.Y)), DragLeft->GetFloatValue(abs(lv.Y)),
		DragTop->GetFloatValue(abs(lv.Z)), DragBottom->GetFloatValue(abs(lv.Z))
	);

	lv = LocalVelocity;
	FVector drag = dragCoefficient.Size() * lv2 * -lv.GetSafeNormal();

	FTransform worldTransform = Rigidbody->GetComponentTransform();
	drag = worldTransform.TransformVector(drag);
	Rigidbody->AddForce(drag);
	CurrentDrag = drag;

}

void UAircraftPhysics::UpdateLift()
{
	if (LocalVelocity.SizeSquared() < 1.0f)
		return;

	FVector liftForce = CalculateLift(AngleOfAttack, FVector(0, 1, 0), LiftPower, AoACurve, InducedDragCurve);

//	Rigidbody->AddForce(liftForce);
}

void UAircraftPhysics::UpdateThrottle(float dt)
{
	// throttle target based on input
	float target = FMath::Clamp(ThrottleInput, 0, 1);
	if(ThrottleInput > 0)
		Throttle = MoveTo(Throttle, target, ThrottleSpeed * abs(ThrottleInput), dt, 0, 1);
	else
		Throttle = MoveTo(Throttle, target, BreakSpeed * abs(ThrottleInput), dt, 0, 1);
		//UE_LOG(LogTemp, Warning, TEXT("Throttle target: %f"), Throttle);
	//UE_LOG(LogTemp, Warning, TEXT("Update Throttle Value: %f"), dt);
	//Throttle = 1;
	//if (ThrottleInput < 0)B
	//UE_LOG(LogTemp, Warning, TEXT("Throttle Input: %f"), ThrottleInput);
	//UE_LOG(LogTemp, Warning, TEXT("Throttle: %f"), Throttle);
	//ThrottleInput = 0;
}

void UAircraftPhysics::UpdateSteering(float dt)
{
	// divide speed by scale to read the right value from the steering curve
	if (ControlInput.Size() < 0.01)
	{
		ControlInput = FVector(0, 0, 0);
	}
	float speed = FMath::Max(0, LocalVelocity.X);
	SteeringPower = SteeringCurve->GetFloatValue(speed);

	GForceScaling = CalculateGLimiter(ControlInput, FMath::DegreesToRadians(TurnSpeed) * SteeringPower);

	FQuat invRotation = Rigidbody->GetRelativeRotation().GetInverse().Quaternion();
	FVector targetAV = ControlInput * TurnSpeed * SteeringPower * GForceScaling;
	//targetAV = invRotation.RotateVector(targetAV);


	FVector currentAV = FMath::RadiansToDegrees(LocalAngularVelocity);

	if (abs(targetAV.SizeSquared() - currentAV.SizeSquared()) < 0.01)
	{
		TorqueApplied = FVector(0, 0, 0);
	}
	else
	{

		TorqueApplied = FVector(
			CalculateSteering(dt, currentAV.X, targetAV.X, TurnAcceleration.X * SteeringPower),
			CalculateSteering(dt, currentAV.Y, targetAV.Y, TurnAcceleration.Y * SteeringPower),
			CalculateSteering(dt, currentAV.Z, targetAV.Z, TurnAcceleration.Z * SteeringPower)
		);
	}
	//TorqueApplied.X = 0;
	//TorqueApplied.Z = 0;
	//CurrentForce.X = gForceScaling;
	//correction
	InputDebug = ControlInput;
	ControlInput = FVector(0, 0, 0);
	TargetAngularVelocity = targetAV;
	CurrentAngularVelocity = currentAV;
	//TorqueApplied = invRotation.RotateVector(TorqueApplied);
	FQuat quaternion = Rigidbody->GetComponentRotation().Quaternion();
	TorqueApplied = quaternion.RotateVector(TorqueApplied);
	Rigidbody->AddTorqueInDegrees(TorqueApplied, NAME_None, true);

}

FVector UAircraftPhysics::CalculateLift(float angleOfAttack, FVector forwardAxis, float liftPower, UCurveFloat* aoACurve, UCurveFloat* inducedDragCurve)
{
	// project velocitty to XZ plane
	if (ThrottleInput > 0)
		int a = 2;
	FVector liftVelocity = FVector::VectorPlaneProject(LocalVelocity, forwardAxis);

	
	float v2 = (liftVelocity / Scale).SizeSquared();

	// lift = velocity^2 * coefficient + liftPower
	float liftCoefficient = aoACurve->GetFloatValue(FMath::RadiansToDegrees(angleOfAttack));
	float liftForce = v2 * liftCoefficient * LiftPower;

	// lift is perpendicular to velocity
	FVector liftDirection = FVector::CrossProduct(liftVelocity.GetSafeNormal(), forwardAxis);
	FVector lift = liftDirection * liftForce;

	CurrentLift.X = liftCoefficient;
	CurrentLift.Y = LiftPower;
	CurrentLift.Z = v2;
	FTransform worldTransform = Rigidbody->GetComponentTransform();
	lift = worldTransform.TransformVector(lift);
	CurrentLift = lift;

	//induced drag varies with square of lift coefficient
	float dragForce = liftCoefficient * liftCoefficient;
	FVector dragDirection = -liftVelocity.GetSafeNormal();
	FVector inducedDrag = dragDirection * v2 * dragForce * InducedDrag * inducedDragCurve->GetFloatValue(FMath::Max(0, LocalVelocity.X));

	CurrentInducedDrag = inducedDrag;

	return lift + inducedDrag;
}

float UAircraftPhysics::CalculateSteering(float dt, float angularVelocity, float targetVelocity, float acceleration)
{
	float error = targetVelocity - angularVelocity;
	float accel = acceleration * dt;

	return FMath::Clamp(error, -accel, accel);
}

FVector UAircraftPhysics::PredictVelocity(FVector force, float dt)
{
	return Velocity + dt * 0.5 * force / Rigidbody->GetMass();
}

FVector UAircraftPhysics::PredictAngularVelocity(FVector force, float dt)
{
	return FVector();
}

FVector UAircraftPhysics::Scale6(FVector value, float posX, float negX, float posY, float negY, float posZ, float negZ)
{
	if (value == FVector::Zero())
		return value;


	if (value.X > 0)
	{
		value.X *= posX;
	}
	else if(value.X < 0)
	{
		value.X *= negX;
	}

	if (value.Y > 0) 
	{
		value.Y *= posY;
	}
	else if (value.Y < 0)
	{
		value.Y *= negY;
	}

	if (value.Z > 0)
	{
		value.Z *= posZ;
	}
	else if (value.Z < 0)
	{
		value.Z *= negZ;
	}

	return value;
}

float UAircraftPhysics::MoveTo(float value, float target, float speed, float deltaTime, float min = 0, float max = 1)
{
	float diff = target - value;
	float delta = FMath::Clamp(diff, -speed * deltaTime, speed * deltaTime);
	return FMath::Clamp(value + delta, min, max);
}

// Called every frame
void UAircraftPhysics::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CalculateState(DeltaTime);
	CalculateGForce(DeltaTime);
	
	UpdateThrottle(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("Update Throttle Value in Tick: %f"), Throttle);
	UpdateThrust();
	UpdateLift();

	UpdateSteering(DeltaTime);

	UpdateDrag();


	// ...
	/*FVector end = Rigidbody->GetComponentLocation() + Rigidbody->GetForwardVector() * 2000;
	DrawDebugLine(GetWorld(), Rigidbody->GetComponentLocation(), end, FColor::Green, false, 0.05f, 0, 1.0f);*/
	//// apply some gravity 
	//Rigidbody->SetPhysicsLinearVelocity(Velocity + FVector(0, 0, -9.8f * DeltaTime * DeltaTime));
//	FVector GravityForce = FVector(0.0f, 0.0f, -GetWorld()->GetGravityZ() * Rigidbody->GetMass());
//	Rigidbody->AddForce(GravityForce);


}

