// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlanePawnPlayer.h"
#include "PlanePawnPlayer.h"
#include "Game/ArenaGameState.h"
#include "PlaneController.h"
#include "NiagaraFunctionLibrary.h"
#include "UI/PlaneHUD.h"
#include "Physics/AircraftPhysics.h"
#include "Components/HealthComponent.h"
void APlanePawnPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TeamID = 0;
}

void APlanePawnPlayer::BeginPlay()
{
	Super::BeginPlay();
	InitalRotation = GetActorRotation();
	InitalLocation = GetActorLocation();
	//AArenaGameState* gameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
	//if (!IsValid(gameState))
	//	return;
	//gameState->AddPlayerToTeam(GameName, TeamID);
}

void APlanePawnPlayer::RespawnPlayer(const FVector& spawnLocation)
{
	PlaneBodyMesh->SetVisibility(false);
	SetActorRotation(InitalRotation);
	SetActorLocation(spawnLocation);
	// reset aircraft physics
	// set initial velocity
	PlanePhysicsComponent->Reset(InitialVelocity);

}
void APlanePawnPlayer::RespawnPlayer()
{
	const bool visibility = true;
	if (PlaneBodyMesh)
		PlaneBodyMesh->SetVisibility(visibility);
	if (PlanePropellerMesh)
		PlanePropellerMesh->SetVisibility(visibility);
	if (LeftLandingGearMesh)
		LeftLandingGearMesh->SetVisibility(visibility);
	if (RightLandingGearMesh)
		RightLandingGearMesh->SetVisibility(visibility);
	if (AileronLeftMesh)
		AileronLeftMesh->SetVisibility(visibility);
	if (AileronRightMesh)
		AileronRightMesh->SetVisibility(visibility);
	if (RudderMesh)
		RudderMesh->SetVisibility(visibility);
	if (ElevatorMesh)
		ElevatorMesh->SetVisibility(visibility);

	SetActorRotation(InitalRotation);
	SetActorLocation(InitalLocation);
	HealthComponent->Reset();
	// reset aircraft physics
	// set initial velocity
	PlanePhysicsComponent->Reset(InitialVelocity);
	APlaneController* controller = GetController<APlaneController>();
	if (controller)
	{
		controller->GetWidgetHUD()->UpdateHealthBar(1);
	}
}

void APlanePawnPlayer::PlaneDeath(AActor* instigator)
{
	const bool visibility = false;
	if(PlaneBodyMesh)
		PlaneBodyMesh->SetVisibility(visibility);
	if(PlanePropellerMesh)
		PlanePropellerMesh->SetVisibility(visibility);
	if(LeftLandingGearMesh)
		LeftLandingGearMesh->SetVisibility(visibility);
	if(RightLandingGearMesh)
		RightLandingGearMesh->SetVisibility(visibility);
	if(AileronLeftMesh)
		AileronLeftMesh->SetVisibility(visibility);
	if(AileronRightMesh)
		AileronRightMesh->SetVisibility(visibility);
	if(RudderMesh)
		RudderMesh->SetVisibility(visibility);
	if(ElevatorMesh)
		ElevatorMesh->SetVisibility(visibility);

	PlanePhysicsComponent->Disable();

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BigExplosionEffect, GetActorLocation());
	APlaneController* controller = GetController<APlaneController>();
	if (controller)
	{
		controller->GetWidgetHUD()->DisplayDeathScreen();
	}
	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &APlanePawnPlayer::RespawnPlayer, 5.0f, false);
}
