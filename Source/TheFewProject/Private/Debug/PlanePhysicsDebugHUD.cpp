// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/PlanePhysicsDebugHUD.h"
#include "PlanePawn.h"
#include "PlanePawnAI.h"
#include "PlaneController.h"
#include "PlaneAIController.h"
#include "Components/BoxComponent.h"
#include "Physics/AircraftPhysics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Game/GameDataUtils.h"
#include "Managers/AIManager.h"
void APlanePhysicsDebugHUD::DrawHUD()
{
	APlanePawn* player = Cast<APlanePawn>(GetOwningPawn());
	if (!IsValid(player))
		return;

	APlaneController* controller = Cast<APlaneController>(player->GetController());

	//if (controller)
	//{
	//	AAIManager* mgr = controller->GetAIManager();
	//	if (mgr)
	//	{
	//		DrawDebugTeamInfo(mgr->TeamA);
	//		X += 400;
	//		Y = 150.0f;
	//		DrawDebugTeamInfo(mgr->TeamB);
	//	}
	//}

	if (controller)
	{
		APlanePawn* ai = controller->GetPlaneSelected();
		if (IsValid(ai))
		{
			APlaneAIController* aiController = ai->GetController<APlaneAIController>();

			AddText(TEXT("Game Name"), FText::FromString(ai->GetGameName()));
			//APlanePawn* target = controller->GetPlaneSelected();
			AddText(TEXT("Name"), FText::FromString(ai->GetName()));
			if (aiController)
			{
				FString enumS = UEnum::GetValueAsString(aiController->CurrentState);
				AddText(TEXT("State"), FText::FromString(enumS));
			}
			DrawSteering(ai);
			DrawThrust(ai);
			DrawPlaneData(ai);
		}
		/*else if (player)
		{

			DrawSteering(player);
			DrawThrust(player);
			DrawPlaneData(player);
		}*/
	}
	//APlane

	//if (player)
	//{

	//	DrawSteering(player);
	//	DrawThrust(player);
	//	DrawPlaneData(player);
	//	
	//}
	X = 250.f;
	Y = 150.0f;
}

void APlanePhysicsDebugHUD::DrawSteering(APlanePawn* pl)
{
	AddText(TEXT("--- STEERING DATA"), FText());
	AddFloat(TEXT("Throttle "), pl->PlanePhysicsComponent->Throttle, FLinearColor::Red);

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
	//AddFloat(TEXT("AoA Yaw"), FMath::RadiansToDegrees(pl->PlanePhysicsComponent->AngleOfAttackYaw));
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
	if(player->PlaneBodyBox)
		AddFloat(TEXT("Altitude"), player->PlaneBodyBox->GetComponentLocation().Z);
	//FRotator rot = player->PlaneBodyBox->GetComponentRotation();
	FRotator rot = player->GetActorRotation();
	FVector temp = FVector(rot.Roll, rot.Pitch, rot.Yaw);
	if (player->PlanePhysicsComponent)
	{
		AddVector(TEXT("Player Rotation"), temp);
		AddVector(TEXT("Local Velocity"), player->PlanePhysicsComponent->LocalVelocity);
		AddVector(TEXT("Velocity"), player->PlanePhysicsComponent->Velocity);
		AddFloat(TEXT("Velocity"), player->PlanePhysicsComponent->Velocity.Size());
		AddVector(TEXT("Forward Vector"), player->PlanePhysicsComponent->Rigidbody->GetForwardVector());

	}
}

void APlanePhysicsDebugHUD::DrawDebugTeamInfo(FTeam& team)
{
	AddText(TEXT("Name Team"), FText::FromString(team.TeamName));
	AddText(TEXT("Player"), FText::FromString("Target"));
	for (auto& actor : team.AIActors)
	{
		AddText(TEXT(""), FText::FromString(actor->GetGameName()));
		APlaneAIController* controller = Cast<APlaneAIController>(actor->GetController());
		if (controller)
		{
			Y -= LineHeight;
			X += 80;
			AddText(TEXT(""), FText::FromString(controller->GetTargetActorName()));
			X -= 80;
		}
	}
}
