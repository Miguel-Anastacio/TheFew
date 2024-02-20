#include "AIManager.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/AIManager.h"
#include "PlanePawnAI.h"
#include "PlaneAIController.h"
#include "PlaneController.h"
#include "Kismet/GameplayStatics.h"
#include "Game/ArenaGameState.h"
#include "Components/SlateWrapperTypes.h"

// Sets default values
AAIManager::AAIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAIManager::IncreaseTeamScore(int teamID)
{
	/*if (teamID == TeamA.ID)
	{
		TeamA.Score++;
	}
	else if (teamID == TeamB.ID)
	{
		TeamB.Score++;
	}*/
}

void AAIManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	for (auto& tgt : Targets)
	{
		tgt->SetOwner(this);
	}

	//AArenaGameState* gameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
	//if (!IsValid(gameState))
	//	return;
	//gameState->InitTeamID(TeamA, TeamB);
	//gameState->InitTeamData(TeamA, TeamB);
}

// Called when the game starts or when spawned
void AAIManager::BeginPlay()
{
	Super::BeginPlay();
	APlaneController* controllerPlayer = Cast<APlaneController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	controllerPlayer->InitDebugVariables(LevelLandscape, this);

	SpawnTeam(TeamA);
	SpawnTeam(TeamB, FRotator(0, 180, 0));

	APlanePawn* player = Cast<APlanePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!IsValid(player))
		return;
	TeamA.AIActors.Add(player);


	AArenaGameState* gameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
	if (!IsValid(gameState))
		return;
	gameState->InitTeamData(TeamA, TeamB);

	InitTeamTargets(TeamA, TeamB);

	CurrentTeamID = TeamB.ID;
}
void AAIManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	//GetWorld()->GetTimerManager().ClearTimer(TeamB.SpawnTimerHandler);

}
// Called every frame
void AAIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (PlaneSelectedIndex > -1 && CurrentTeamID == TeamA.ID && PlaneSelectedIndex < TeamA.AIActors.Num() - 1)
	//{
	//	// DEBUG ONLY CODE
	//	APlaneAIController* controller = Cast<APlaneAIController>(TeamA.AIActors[PlaneSelectedIndex]->Controller);
	//	if(controller)
	//		controller->ShowDebugInfo();
	//}
	//else if(PlaneSelectedIndex > -1 && CurrentTeamID == TeamB.ID && PlaneSelectedIndex < TeamB.AIActors.Num()-1)
	//{
	//	// DEBUG ONLY CODE
	//	APlaneAIController* controller = Cast<APlaneAIController>(TeamB.AIActors[PlaneSelectedIndex]->Controller);
	//	if (controller)
	//		controller->ShowDebugInfo();
	//}

	TeamA.TimeSinceLastSpawn += DeltaTime;
	HandleSpawnQueue(TeamA);

	TeamB.TimeSinceLastSpawn += DeltaTime;
	HandleSpawnQueue(TeamB);
}
UE_DISABLE_OPTIMIZATION
void AAIManager::OnAIDestroyed(AActor* actor)
{
	APlanePawnAI* actorAI = Cast<APlanePawnAI>(actor);
	if (!IsValid(actorAI))
		return;

	AArenaGameState* gameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
	if (!IsValid(gameState))
		return;

	int teamID = actorAI->GetTeamID();
	// find planes that had this actor as target 
	// and assign them a new target

	if (teamID == TeamA.ID)
	{
		RemoveElementFromTeam(TeamA, actorAI->GetGameName());

		Respawn(TeamA, TeamB.AIActors, actorAI->GetGameName());
		// find planes that had this actor as target 
		// and assign them a new target
		UpdateTarget(actor, TeamB, TeamA.AIActors);
	}
	else if (teamID == TeamB.ID)
	{
		RemoveElementFromTeam(TeamB, actorAI->GetGameName());
		Respawn(TeamB, TeamA.AIActors, actorAI->GetGameName(), FRotator(0, 180, 0));
		// find planes that had this actor as target 
		// and assign them a new target
		UpdateTarget(actor, TeamA, TeamB.AIActors);
	}
	
}
UE_ENABLE_OPTIMIZATION

void AAIManager::SpawnTeam(FTeam& team, const FRotator& rot)
{
	team.InitSpawnAreaBounds();
	for (int i = 0; i < team.NumberOfAIActors; i++)
	{
		//SpawnAIActor(team.SpawnAreaBoundsMin, team.SpawnAreaBoundsMax, team.SpawnHeight);
		SpawnAIActor(team, FString(), rot);
	}
}

void AAIManager::InitTeamTargets(FTeam& team1, FTeam& team2)
{
	
	for (auto& actor : team1.AIActors)
	{
		APlaneAIController* controller = actor->GetController<APlaneAIController>();
		if (controller)
		{
			int index = FMath::RandRange(0, team2.AIActors.Num() - 1);
			controller->SetTargetActor(team2.AIActors[index]);
			controller->SetPatrolDestination(PatrolDestination);
		}

	}

	for (auto& actor : team2.AIActors)
	{
		APlaneAIController* controller = actor->GetController<APlaneAIController>();
		if (controller)
		{
			int index = FMath::RandRange(0, team1.AIActors.Num() - 1);
			controller->SetTargetActor(team1.AIActors[index]);
			controller->SetPatrolDestination(PatrolDestination);
		}

	}
}

void AAIManager::UpdateTarget(AActor* currentTarget, FTeam& teamToUpdate, const TArray<TObjectPtr<class APlanePawn>>& newTargets)
{
	for (auto& temp : teamToUpdate.AIActors)
	{
		APlaneAIController* controller = temp->GetController<APlaneAIController>();
		if (controller)
		{
			if (currentTarget == controller->GetTargetActor())
			{
				int index = FMath::RandRange(0, newTargets.Num() - 1);
				controller->SetTargetActor(newTargets[index]);
			}
		}
	}
}

void AAIManager::RemoveElementFromTeam(FTeam& team, const FString& elementName)
{
	for (int i = 0; i < team.AIActors.Num(); i++)
	{
		if (elementName == team.AIActors[i]->GetGameName())
		{
			team.AIActors.RemoveAt(i);
		}
	}
}

void AAIManager::HandleSpawnQueue(FTeam& team)
{
	if (team.TimeSinceLastSpawn > team.SpawnCooldown)
	{
		if (team.SpawnQueue.IsEmpty())
			return;
		FSpawnData data = team.SpawnQueue.Pop();
		SpawnAIActorGameInProgress(team, *data.EnemyTeamActors, data.Name, data.Rot);
	}
}

TObjectPtr<class APlanePawn> AAIManager::ChangePlaneSelected(float input)
{
	/*if (TeamB.AIActors.IsValidIndex(PlaneSelectedIndex))
	{
		APlaneAIController* controller = TeamB.AIActors[PlaneSelectedIndex]->GetController<APlaneAIController>();
		if (controller)
		{
			controller->SetTargetActorVisibility(ESlateVisibility::Collapsed);
		}
	}*/
	if (CurrentTeamID == TeamB.ID)
	{
		/*for (int i = 0; i < TeamA.AIActors.Num(); i++)
		{
			if (!IsValid(TeamA.AIActors[i]))
			{
				TeamA.AIActors.RemoveAt(i);
			}
		}*/

		PlaneSelectedIndex += input;
		if (PlaneSelectedIndex >= TeamB.AIActors.Num())
		{
			PlaneSelectedIndex = 0;
			//CurrentTeamID = TeamB.ID;
		}
		else if (PlaneSelectedIndex < 0)
		{
			PlaneSelectedIndex = TeamB.AIActors.Num() - 1;
			//CurrentTeamID = TeamB.ID;
		}


	}

	//APlaneAIController* controller = TeamB.AIActors[PlaneSelectedIndex]->GetController<APlaneAIController>();
	//if (controller)
	//{
	//	controller->SetTargetActorVisibility(ESlateVisibility::HitTestInvisible);
	//}
	return TeamB.AIActors[PlaneSelectedIndex];
	//else
	//{

	//	for (int i = 0; i < TeamB.AIActors.Num(); i++)
	//	{
	//		if (!IsValid(TeamB.AIActors[i]))
	//		{
	//			TeamB.AIActors.RemoveAt(i);
	//		}
	//	}

	//	PlaneSelectedIndex += input;
	//	if (PlaneSelectedIndex >= TeamB.AIActors.Num())
	//	{
	//		PlaneSelectedIndex = 0;
	//		CurrentTeamID = TeamA.ID;
	//	}
	//	else if (PlaneSelectedIndex < 0)
	//	{
	//		PlaneSelectedIndex = TeamB.AIActors.Num() - 1;
	//		CurrentTeamID = TeamA.ID;
	//	}

	//	return TeamB.AIActors[PlaneSelectedIndex];
	//}
	// 
	
	//return NULL;
}

//void AAIManager::SpawnAIActor(const FVector2D& minBounds, const FVector2D& maxBounds, float zHeight, )
//{
//	if (IsValid(AIClass))
//	{
//		FActorSpawnParameters params;
//		FVector loc;
//		loc.X = FMath::RandRange(minBounds.X, maxBounds.X);
//		loc.Y = FMath::RandRange(minBounds.Y, maxBounds.Y);
//		loc.Z = zHeight;
//		FHitResult hit;
//		GetWorld()->LineTraceSingleByChannel(hit, loc, loc + FVector::DownVector * 1000000, ECC_Visibility);
//		int maxTries = 10;
//		int counter = 0;
//		while (!hit.bBlockingHit && counter < maxTries)
//		{
//			loc.X = FMath::RandRange(minBounds.X, maxBounds.X);
//			loc.Y = FMath::RandRange(minBounds.Y, maxBounds.Y);
//			GetWorld()->LineTraceSingleByChannel(hit, loc, loc + FVector::DownVector * 100000, ECC_Visibility);
//			counter++;
//		}
//
//		if (counter > maxTries)
//		{
//			return;
//		}
//
//		TObjectPtr<APlanePawnAI> enemy = GetWorld()->SpawnActor<APlanePawnAI>(AIClass, loc, FRotator(0, 0, 0), params);
//		if (IsValid(enemy))
//		{
//			enemy->SetOwner(this);
//			enemy->OnDestroyed.AddDynamic(this, &AAIManager::OnAIDestroyed);
//			UpdateTarget(enemy);
//			AIActors.Add(enemy);
//		}
//	}
//}
UE_DISABLE_OPTIMIZATION
void AAIManager::SpawnAIActor(FTeam& team, const FString& playerName, const FRotator& rot)
{
	if (IsValid(team.AIClass))
	{
		FActorSpawnParameters params;
		FVector loc;
		loc.X = FMath::RandRange(team.SpawnAreaBoundsMin.X, team.SpawnAreaBoundsMax.X);
		loc.Y = FMath::RandRange(team.SpawnAreaBoundsMin.Y, team.SpawnAreaBoundsMax.Y);
		loc.Z = team.SpawnHeight;
		FHitResult hit;
		//GetWorld()->LineTraceSingleByChannel(hit, loc, loc + FVector::DownVector * 1000000, ECC_Visibility);
		int maxTries = 10;
		int counter = 0;

		// not good 
		TObjectPtr<APlanePawnAI> enemy = GetWorld()->SpawnActor<APlanePawnAI>(team.AIClass, loc, rot, params);
		while (!IsValid(enemy) && counter < maxTries)
		{
			loc.X = FMath::RandRange(team.SpawnAreaBoundsMin.X, team.SpawnAreaBoundsMax.X);
			loc.Y = FMath::RandRange(team.SpawnAreaBoundsMin.Y, team.SpawnAreaBoundsMax.Y);
			enemy = GetWorld()->SpawnActor<APlanePawnAI>(team.AIClass, loc, rot, params);
			//GetWorld()->LineTraceSingleByChannel(hit, loc, loc + FVector::DownVector * 100000, ECC_Visibility);
			counter++;
		}

		if (counter > maxTries)
		{
			return;
		}

		if (IsValid(enemy))
		{
			enemy->SetOwner(this);
			enemy->OnDestroyed.AddDynamic(this, &AAIManager::OnAIDestroyed);
			enemy->SetWidgetColor(team.AIWidgetColor);

			FString name = playerName;
			if (playerName == FString())
			{
				name = team.AIGameName;
				name.AppendInt(team.AIActors.Num());

			}
			enemy->SetGameName(name);
			enemy->SetTeamID(team.ID);
			//UpdateTarget(enemy);
			team.AIActors.Add(enemy);
			team.TimeSinceLastSpawn = 0.0f;
		}
	}
}
UE_ENABLE_OPTIMIZATION

void AAIManager::UpdateTarget(AActor* actor)
{
	//APlanePawnAI* enemy = Cast<APlanePawnAI>(actor);
	//if (enemy)
	//{
	//	APlaneAIController* controller = enemy->GetController<APlaneAIController>();
	//	if (controller)
	//	{
	//		int index = FMath::RandRange(0, Targets.Num() - 1);
	//		controller->SetTargetActor(Targets[index]);
	//	}
	//}
}

UE_DISABLE_OPTIMIZATION
void AAIManager::SpawnAIActorGameInProgress(FTeam& team, const TArray<class APlanePawn*>& enemyTeamActors, const FString& playerName, const FRotator& rot, bool timer)
{
	if (timer)
		int a = 0;
	SpawnAIActor(team, playerName, rot);
	APlaneAIController* controller = team.AIActors.Last()->GetController<APlaneAIController>();
	if (controller)
	{
		int index = FMath::RandRange(0, enemyTeamActors.Num() - 1);

		controller->SetPatrolDestination(PatrolDestination);
		controller->SetTargetActor(enemyTeamActors[index]);
		if (!IsValid(controller->GetTargetActor()))
		{
			int  a = 0;
		}
	}
}

UE_ENABLE_OPTIMIZATION
void AAIManager::Respawn(FTeam& team, const TArray<TObjectPtr<class APlanePawn>>& enemyTeamActors, const FString& playerName, const FRotator& rot)
{
	if (team.TimeSinceLastSpawn > team.SpawnCooldown)
	{
		SpawnAIActorGameInProgress(team, enemyTeamActors, playerName, rot, false);
	}
	else
	{
		//// start a timer to spawn an actor
		//FTimerDelegate Delegate; // Delegate to bind function with parameters
		//bool timer = true;
		//Delegate.BindUFunction(this, "SpawnAIActorGameInProgress", team, enemyTeamActors, playerName, rot, timer);
		//float delay = team.SpawnCooldown - team.TimeSinceLastSpawn;
		//if (GetWorld()->GetTimerManager().IsTimerActive(team.SpawnTimerHandler))
		//{
		//	FTimerHandle newTimer;
		//	GetWorld()->GetTimerManager().SetTimer(newTimer, Delegate, delay, false);
		//}
		//else
		//{
		//	GetWorld()->GetTimerManager().SetTimer(team.SpawnTimerHandler, Delegate, delay, false);

		//}

		team.SpawnQueue.Push(FSpawnData(&enemyTeamActors, playerName, rot));
	}


}

