// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlanePawnPlayer.h"
#include "PlanePawnPlayer.h"
#include "Game/ArenaGameState.h"
#include "PlaneController.h"
#include "NiagaraFunctionLibrary.h"
#include "UI/PlaneHUD.h"
#include "Physics/AircraftPhysics.h"
#include "Components/HealthComponent.h"
#include "Components/PrimitiveComponent.h"
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
	PlaneDeathSimple();
}

void APlanePawnPlayer::RespawnPlayer(const FVector& spawnLocation)
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
	SetActorLocation(spawnLocation);
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

void APlanePawnPlayer::PlaneDeathSimple()
{
	const bool visibility = false;
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

	PlanePhysicsComponent->Disable();
	//this->actove

	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BigExplosionEffect, GetActorLocation());
	/*APlaneController* controller = GetController<APlaneController>();
	if (controller)
	{
		controller->GetWidgetHUD()->DisplayDeathScreen();
	}
	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &APlanePawnPlayer::RespawnPlayer, 5.0f, false);*/
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
	//FTimerHandle timer;
	//GetWorld()->GetTimerManager().SetTimer(timer, this, &APlanePawnPlayer::RespawnPlayer, 5.0f, false);

	AArenaGameState* gameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
	if (!IsValid(gameState))
	{
		return;
	}
	APlanePawn* other = Cast<APlanePawn>(instigator);
	if (other)
	{
		gameState->UpdateScoreboard(other->GetGameName(), this->GameName);
	}
}

void APlanePawnPlayer::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		if (OtherActor->ActorHasTag("Bounds"))
		{
			OverlappedBounds.Add(OtherComp->GetName(), OtherComp);
			APlaneController* controller = GetController<APlaneController>();
			if (controller)
			{
				FTimerManager& mgr = GetWorld()->GetTimerManager();
				if (!mgr.IsTimerActive(OutOfBoundsTimerHandle))
				{
					controller->GetWidgetHUD()->DisplayOutOfBoundsWidget();
					float lifetime = controller->GetWidgetHUD()->GetLifetimeOfOutOfBoundsWidget();
					FTimerDelegate Delegate;
					Delegate.BindUFunction(this, "PlaneDeath", OtherActor);
					GetWorld()->GetTimerManager().SetTimer(OutOfBoundsTimerHandle, Delegate, lifetime, false);
				}
			}
		}
	}
}

void APlanePawnPlayer::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor))
	{
		if (OtherActor->ActorHasTag("Bounds"))
		{
			OverlappedBounds.Remove(OtherComp->GetName());

			// if it is last bound then remove widget and clear timer
			if (!OverlappedBounds.IsEmpty())
				return;

			APlaneController* controller = GetController<APlaneController>();
			if (controller)
			{
				controller->GetWidgetHUD()->RemoveOutOfBoundsWidget();
			}

			FTimerManager& mgr = GetWorld()->GetTimerManager();
			if (mgr.IsTimerActive(OutOfBoundsTimerHandle))
			{
				mgr.ClearTimer(OutOfBoundsTimerHandle);
			}
		}
	}
}
