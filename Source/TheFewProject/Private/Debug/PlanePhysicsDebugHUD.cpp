// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/PlanePhysicsDebugHUD.h"
#include "PlanePawn.h"
#include "PlaneController.h"
#include "Components/BoxComponent.h"
#include "Physics/AircraftPhysics.h"
#include "GameFramework/SpringArmComponent.h"

void APlanePhysicsDebugHUD::DrawHUD()
{

	APlanePawn* player = Cast<APlanePawn>(GetOwningPawn());
	
	APlaneController* controller = Cast<APlaneController>(player->GetController());
	if (controller)
	{
		APlanePawn* ai = controller->GetPlaneSelected();
		if (ai)
		{
			DrawSteering(ai);
			DrawThrust(ai);
			DrawPlaneData(ai);
		}
	}
	//APlane

	if (player)
	{
		////AddFloat(TEXT("Thrust"), player->PlanePhysicsComponent->CurrentForce.X);
		//AddFloat(TEXT("Targuet Quat X"), player->TargetCameraRotation.X);
		//AddFloat(TEXT("Targuet Quat Y"), player->TargetCameraRotation.Y);
		//AddFloat(TEXT("Targuet Quat Z"), player->TargetCameraRotation.Z);
		//AddFloat(TEXT("Targuet Quat W"), player->TargetCameraRotation.W);
		//AddVector(TEXT("Target Rot "), player->TargetCameraRotation.Euler());
		////AddVector(TEXT("Induced Drag"), player->PlanePhysicsComponent->CurrentInducedDrag);
		//AddVector(TEXT("G Force "), player->PlanePhysicsComponent->LocalGForce);
		//AddVector(TEXT("Drag"), player->PlanePhysicsComponent->CurrentDrag);
		//AddVector(TEXT("Lift"), player->PlanePhysicsComponent->CurrentLift);
		//AddVector(TEXT("LocalVelocity"), player->PlanePhysicsComponent->LocalVelocity);
		//AddFloat(TEXT("Angle of Attack"), FMath::RadiansToDegrees(player->PlanePhysicsComponent->AngleOfAttack));
		//AddFloat(TEXT("Throttle"), player->PlanePhysicsComponent->Throttle);
		//AddFloat(TEXT("Altitude"), player->PlaneBodyBox->GetComponentLocation().Z);
		//FRotator rot = player->PlaneBodyBox->GetComponentRotation();
		//FVector temp = FVector(rot.Roll, rot.Pitch, rot.Yaw);
		//AddVector(TEXT("Rotation"), temp);

		/*DrawSteering(player);
		DrawThrust(player);
		DrawPlaneData(player);*/
		
	}
	Y = 150.0f;
}

void APlanePhysicsDebugHUD::DrawSteering(APlanePawn* pl)
{
	AddText(TEXT("--- STEERING DATA"), FText());
	AddVector(TEXT("Current Angular Velocity"), pl->PlanePhysicsComponent->CurrentAngularVelocity);
	AddVector(TEXT("Target Angular Velocity"), pl->PlanePhysicsComponent->TargetAngularVelocity);
	AddVector(TEXT("Steering Torque"), pl->PlanePhysicsComponent->TorqueApplied);
	AddVector(TEXT("G Force "), pl->PlanePhysicsComponent->LocalGForce);
	AddFloat(TEXT("G Limit Scaling "), pl->PlanePhysicsComponent->GForceScaling);
	AddVector(TEXT("G Force Limit "), pl->PlanePhysicsComponent->GForceLimitDebug);
	AddVector(TEXT("Control Input "), pl->PlanePhysicsComponent->ControlInput);
	AddFloat(TEXT("Steering Power "), pl->PlanePhysicsComponent->SteeringPower);
	AddFloat(TEXT("AoA "), FMath::RadiansToDegrees(pl->PlanePhysicsComponent->AngleOfAttack));
	AddFloat(TEXT("AoA Yaw"), FMath::RadiansToDegrees(pl->PlanePhysicsComponent->AngleOfAttackYaw));
	AddFloat(TEXT("AoA Yaw"), FMath::RadiansToDegrees(pl->PlanePhysicsComponent->AngleOfAttackYaw));
	AddVector(TEXT("Angular Drag"), pl->PlanePhysicsComponent->CurrentAngularDrag);


}

void APlanePhysicsDebugHUD::DrawThrust(APlanePawn* player)
{
	AddText(TEXT("--- FORCES "), FText());
	AddVector(TEXT("Thrust"), player->PlanePhysicsComponent->CurrentForce);
	AddVector(TEXT("Induced Drag"), player->PlanePhysicsComponent->CurrentInducedDrag);
	AddVector(TEXT("Drag"), player->PlanePhysicsComponent->CurrentDrag);
	AddVector(TEXT("Lift"), player->PlanePhysicsComponent->CurrentLift);
	AddVector(TEXT("Local Lift"), player->PlanePhysicsComponent->CurrentLocalLift);
	AddVector(TEXT("Yaw Lift "), player->PlanePhysicsComponent->CurrnetYawLift);
	AddVector(TEXT("Drag Coef"), (player->PlanePhysicsComponent->DragCoefficient));

}

void APlanePhysicsDebugHUD::DrawPlaneData(APlanePawn* player)
{
	AddText(TEXT("--- TRANSFORM DATA"), FText());

	AddFloat(TEXT("Altitude"), player->PlaneBodyBox->GetComponentLocation().Z);
	//FRotator rot = player->PlaneBodyBox->GetComponentRotation();
	FRotator rot = player->GetActorRotation();
	FVector temp = FVector(rot.Roll, rot.Pitch, rot.Yaw);
	AddVector(TEXT("Player Rotation"), temp);
	AddVector(TEXT("Local Velocity"), player->PlanePhysicsComponent->LocalVelocity);
	AddVector(TEXT("Velocity"), player->PlanePhysicsComponent->Velocity);
	AddFloat(TEXT("Velocity"), player->PlanePhysicsComponent->Velocity.Size());
	AddVector(TEXT("Forward Vector"), player->PlanePhysicsComponent->Rigidbody->GetForwardVector());
}
