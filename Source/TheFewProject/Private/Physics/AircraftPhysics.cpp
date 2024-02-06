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
	Flaps = true;
	UE_LOG(LogProjectFew, Log, TEXT("Hello Custom Log"));
	// ...
	//LocalVelocity = FVector(800, 35, -140);
	//AngleOfAttack
	//FVector liftForce = CalculateLift(AngleOfAttack, FVector(0, 1, 0), LiftPower, AoACurve, InducedDragCurve);
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


	FTransform localTransform = Rigidbody->GetComponentTransform();
	LocalVelocity = invRotation.RotateVector(Velocity);
	LocalVelocity = localTransform.InverseTransformVector(Velocity);


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

	FVector end = Rigidbody->GetComponentLocation() + thrust;
	//DrawDebugLine(GetWorld(), Rigidbody->GetComponentLocation(), end, FColor::White , false, 0.05f, 0, 1.0f);
	
}

void UAircraftPhysics::UpdateDrag()
{
	FVector lv = LocalVelocity;
	float lv2 = lv.SizeSquared();
	lv2 /= 2;
	//CurrentInducedDrag.Y = lv2;
	// adjust based on scale to interact with curves
	//lv /= Scale;
	//UE_LOG(LogTemp, Warning, TEXT("LocalVelocityDrag: X=%f, Y=%f, Z=%f"), lv.X, lv.Y, lv.Z);
	//lv /= Scale;
	float temp = 1;
	FVector dragCoefficient = Scale6(
		lv.GetSafeNormal(),
		DragForward->GetFloatValue(abs(lv.X)* temp) , DragBack->GetFloatValue(abs(lv.X)*temp),
		DragRight->GetFloatValue(abs(lv.Y)* temp), DragLeft->GetFloatValue(abs(lv.Y)*temp),
		DragTop->GetFloatValue(abs(lv.Z)* temp), DragBottom->GetFloatValue(abs(lv.Z)* temp)
	);

	lv = LocalVelocity;
	FVector drag = dragCoefficient.Size() * lv2 * -lv.GetSafeNormal();

	FQuat quaternion = Rigidbody->GetComponentRotation().Quaternion();
	drag = quaternion.RotateVector(drag);

	Rigidbody->AddForce(drag);
	CurrentDrag = drag;
	FVector end = Rigidbody->GetComponentLocation() + drag;
	//DrawDebugLine(GetWorld(), Rigidbody->GetComponentLocation(), end, FColor::Red, false, 0.05f, 0, 1.0f);
	DragCoefficient = dragCoefficient;
	//DragCoefficient.X = DragForward->GetFloatValue(abs(lv.X));
	//DragCoefficient.X = abs(lv.X);

}

void UAircraftPhysics::UpdateAngularDrag()
{
	FVector av = CurrentAngularVelocity;
	av /= Scale;
	//av.Normalize();
	CurrentAngularDrag = -av.GetSafeNormal() * av.SizeSquared() * AngularDrag;

	FQuat quaternion = Rigidbody->GetComponentRotation().Quaternion();
	CurrentAngularDrag = quaternion.RotateVector(CurrentAngularDrag);

	Rigidbody->AddTorqueInDegrees(CurrentAngularDrag, NAME_None, true);
}

void UAircraftPhysics::UpdateLift()
{
	if (LocalVelocity.SizeSquared() < 1.0f)
		return;
	float flapsliftPower = 0;
	if (Flaps)
		flapsliftPower = this->FlapsLiftPower;
	FVector yawForce = CalculateLift(AngleOfAttackYaw, FVector(0, 0, 1), RudderPower, RudderAoACurve, RudderInducedDragCurve);
	FVector liftForce = CalculateLift(AngleOfAttack, FVector(0, 1, 0), LiftPower + flapsliftPower, AoACurve, InducedDragCurve);
	CurrentLift = liftForce;

	CurrnetYawLift = yawForce;
	DrawDebugForce(CurrentLift, FColor::Yellow);
	DrawDebugForce(CurrentDrag, FColor::Red);
	DrawDebugForce(CurrentForce, FColor::White);

	Rigidbody->AddForce(liftForce);
	Rigidbody->AddForce(yawForce);
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
		return;
	}
	float speed = FMath::Max(0, abs(LocalVelocity.X));
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
	FVector liftVelocity = FVector::VectorPlaneProject(LocalVelocity, forwardAxis);
	//DrawDebugForce(liftVelocity, FColor::Cyan);
	
	float v2 = liftVelocity.SizeSquared();
	v2 /= 2;
	//CurrentInducedDrag.X = v2;
	// lift = velocity^2 * coefficient * liftPower
	float liftCoefficient = aoACurve->GetFloatValue(FMath::RadiansToDegrees(angleOfAttack));

	float liftForce = v2 * liftCoefficient * liftPower;
	//CurrentInducedDrag.Z = liftCoefficient;
	//CurrentInducedDrag.Y = LiftPower;

	//liftVelocity.Normalize();
	// lift is perpendicular to velocity
	FVector liftDirection = FVector::CrossProduct(liftVelocity.GetSafeNormal(), forwardAxis);
	
	//if (abs(liftDirection.Dot(liftVelocity)) <= 0.01)
	//	__debugbreak();
	FVector lift = liftDirection * liftForce;
	//CurrentInducedDrag = liftDirection.GetSafeNormal();

	//CurrentLift.X = liftCoefficient;
	//CurrentLift.Y = LiftPower;
	//CurrentLift.Z = v2;
	//FTransform worldTransform = Rigidbody->GetComponentTransform();
	//lift = worldTransform.TransformVector(lift);

	
	FQuat quaternion = Rigidbody->GetComponentRotation().Quaternion();
	lift = quaternion.RotateVector(lift);
	CurrentLift = lift;

	//induced drag varies with square of lift coefficient
	float dragForce = liftCoefficient * liftCoefficient;
	FVector dragDirection = -liftVelocity.GetSafeNormal();
	FVector inducedDrag = dragDirection * v2 * dragForce * InducedDrag * inducedDragCurve->GetFloatValue(FMath::Max(0, LocalVelocity.X));
	//FQuat quaternion = Rigidbody->GetComponentRotation().Quaternion();
	inducedDrag = quaternion.RotateVector(inducedDrag);
	//CurrentInducedDrag = inducedDrag;
	//DrawDebugForce(inducedDrag)
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

void UAircraftPhysics::DrawDebugForce(FVector force, FColor color)
{
	FVector end = Rigidbody->GetCenterOfMass() + force;
	//DrawDebugLine(GetWorld(), Rigidbody->GetCenterOfMass(), end, color, false, 0.05f, 0, 1.0f);
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
	UpdateAngularDrag();


	// ...
	FVector end = Rigidbody->GetComponentLocation() + Rigidbody->GetPhysicsLinearVelocity();
	//DrawDebugLine(GetWorld(), Rigidbody->GetComponentLocation(),end , FColor::Green, false, 0.05f, 0, 1.0f);
	//// apply some gravity 
	//Rigidbody->SetPhysicsLinearVelocity(Rigidbody->GetPhysicsLinearVelocity() + FVector(0, 0, -980.0f * DeltaTime));
	//FVector GravityForce = FVector(0.0f, 0.0f, GetWorld()->GetGravityZ() * Scale);
	//Rigidbody->AddForce(GravityForce);

	DrawDebugForce(Rigidbody->GetPhysicsLinearVelocity(), FColor::Green);
	DrawDebugForce(FVector(0, 0, GetWorld()->GetGravityZ()), FColor::Black);
	CalculateState(DeltaTime);

}

