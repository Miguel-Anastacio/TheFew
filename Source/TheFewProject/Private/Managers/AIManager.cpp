#include "AIManager.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/AIManager.h"
#include "PlanePawnAI.h"
#include "PlaneAIController.h"
#include "PlaneController.h"
#include "Kismet/GameplayStatics.h"
#include "Game/ArenaGameState.h"
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
}

// Called when the game starts or when spawned
void AAIManager::BeginPlay()
{
	Super::BeginPlay();
	APlaneController* controllerPlayer = Cast<APlaneController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	controllerPlayer->InitDebugVariables(LevelLandscape, this);

	/*if (SpawnArea)
	{
		FVector origin, extent;
		SpawnArea->GetActorBounds(false, origin, extent);
		SpawnAreaBoundsMax.X = origin.X + extent.X;
		SpawnAreaBoundsMax.Y = origin.Y + extent.Y;

		SpawnAreaBoundsMin.X = origin.X - extent.X;
		SpawnAreaBoundsMin.Y = origin.Y - extent.Y;

	}*/

	/*for (int i = 0; i < NumberOfEnemies; i++)
	{
		SpawnAIActor(SpawnAreaBoundsMin, SpawnAreaBoundsMax, SpawnHeight);
	}*/

	SpawnTeam(TeamA);
	SpawnTeam(TeamB, FRotator(0, 180, 0));

	// Each team member targets another one in the other team
	if (TeamA.AIActors.Num() <= TeamB.AIActors.Num())
	{
		for (int i = 0; i < TeamA.AIActors.Num(); i++)
		{
			APlaneAIController* controller = TeamA.AIActors[i]->GetController<APlaneAIController>();
			if (controller)
			{
				/*int index = FMath::RandRange(0, Targets.Num() - 1);*/
				controller->SetTargetActor(TeamB.AIActors[i]);
			}

			controller = TeamB.AIActors[i]->GetController<APlaneAIController>();
			if (controller)
			{
				/*int index = FMath::RandRange(0, Targets.Num() - 1);*/
				controller->SetTargetActor(TeamA.AIActors[TeamA.AIActors.Num()-1 - i]);
			}
		}
	}
	CurrentTeamID = TeamA.ID;

	AArenaGameState* gameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
	if (!IsValid(gameState))
		return;

	gameState->InitTeamData(TeamA, TeamB);

}
void AAIManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(TeamA.SpawnTimerHandler);
	GetWorld()->GetTimerManager().ClearTimer(TeamB.SpawnTimerHandler);

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
	TeamB.TimeSinceLastSpawn += DeltaTime;
}

void AAIManager::OnAIDestroyed(AActor* actor)
{
	APlanePawnAI* actorAI = Cast<APlanePawnAI>(actor);
	if (!IsValid(actorAI))
		return;


	AArenaGameState* gameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
	if (!IsValid(gameState))
		return;

	int teamID = gameState->GetPlayerTeamID(actorAI->GetGameName());
	// find planes that had this actor as target 
	// and assign them a new target

	if (teamID == TeamA.ID)
	{
		// find planes that had this actor as target 
		// and assign them a new target
		// To DO : Create a function for this
		for (auto& temp : TeamB.AIActors)
		{
			APlaneAIController* controller = temp->GetController<APlaneAIController>();
			if (controller)
			{
				/*int index = FMath::RandRange(0, Targets.Num() - 1);*/
				if (actor == controller->GetTargetActor())
				{
					int index = FMath::RandRange(0, TeamA.AIActors.Num() - 1);
					controller->SetTargetActor(TeamA.AIActors[index]);
				}
			}
		}
		Respawn(TeamA, TeamB.AIActors, actorAI->GetGameName());
	}
	else if (teamID == TeamB.ID)
	{
		// find planes that had this actor as target 
		// and assign them a new target
		// To DO : Create a function for this
		for (auto& temp : TeamA.AIActors)
		{
			APlaneAIController* controller = temp->GetController<APlaneAIController>();
			if (controller)
			{
				/*int index = FMath::RandRange(0, Targets.Num() - 1);*/
				if (actor == controller->GetTargetActor())
				{
					int index = FMath::RandRange(0, TeamB.AIActors.Num() - 1);
					controller->SetTargetActor(TeamB.AIActors[index]);
				}
			}
		}
		Respawn(TeamB, TeamA.AIActors, actorAI->GetGameName(), FRotator(0, 180, 0));
	}
	
}

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
	// Each team member targets another one in the other team
	if (team1.AIActors.Num() <= team2.AIActors.Num())
	{
		for (int i = 0; i < team1.AIActors.Num(); i++)
		{
			APlaneAIController* controller = team1.AIActors[i]->GetController<APlaneAIController>();
			if (controller)
			{
				/*int index = FMath::RandRange(0, Targets.Num() - 1);*/
				controller->SetTargetActor(team2.AIActors[i]);
			}

			controller = team2.AIActors[i]->GetController<APlaneAIController>();
			if (controller)
			{
				//int index = FMath::RandRange(0, Targets.Num() - 1);
				controller->SetTargetActor(team1.AIActors[team1.AIActors.Num() - 1 - i]);
			}
		}
	}
}

TObjectPtr<class APlanePawnAI> AAIManager::ChangePlaneSelected(float input)
{
	if (CurrentTeamID == TeamA.ID)
	{
		for (int i = 0; i < TeamA.AIActors.Num(); i++)
		{
			if (!IsValid(TeamA.AIActors[i]))
			{
				TeamA.AIActors.RemoveAt(i);
			}
		}

		PlaneSelectedIndex += input;
		if (PlaneSelectedIndex >= TeamA.AIActors.Num())
		{
			PlaneSelectedIndex = 0;
			CurrentTeamID = TeamB.ID;
		}
		else if (PlaneSelectedIndex < 0)
		{
			PlaneSelectedIndex = TeamA.AIActors.Num() - 1;
			CurrentTeamID = TeamB.ID;
		}

		return TeamA.AIActors[PlaneSelectedIndex];

	}
	else
	{

		for (int i = 0; i < TeamB.AIActors.Num(); i++)
		{
			if (!IsValid(TeamB.AIActors[i]))
			{
				TeamB.AIActors.RemoveAt(i);
			}
		}

		PlaneSelectedIndex += input;
		if (PlaneSelectedIndex >= TeamB.AIActors.Num())
		{
			PlaneSelectedIndex = 0;
			CurrentTeamID = TeamA.ID;
		}
		else if (PlaneSelectedIndex < 0)
		{
			PlaneSelectedIndex = TeamB.AIActors.Num() - 1;
			CurrentTeamID = TeamA.ID;
		}

		return TeamB.AIActors[PlaneSelectedIndex];
	}
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
		GetWorld()->LineTraceSingleByChannel(hit, loc, loc + FVector::DownVector * 1000000, ECC_Visibility);
		int maxTries = 10;
		int counter = 0;
		/*while (!hit.bBlockingHit && counter < maxTries)
		{
			loc.X = FMath::RandRange(minBounds.X, maxBounds.X);
			loc.Y = FMath::RandRange(minBounds.Y, maxBounds.Y);
			GetWorld()->LineTraceSingleByChannel(hit, loc, loc + FVector::DownVector * 100000, ECC_Visibility);
			counter++;
		}

		if (counter > maxTries)
		{
			return;
		}*/

		TObjectPtr<APlanePawnAI> enemy = GetWorld()->SpawnActor<APlanePawnAI>(team.AIClass, loc, rot, params);

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
			//UpdateTarget(enemy);
			team.AIActors.Add(enemy);
			team.TimeSinceLastSpawn = 0.0f;
		}
	}
}

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

void AAIManager::SpawnAIActorGameInProgress(FTeam& team, const TArray<APlanePawnAI*>& enemyTeamActors, const FString& playerName, const FRotator& rot)
{
	SpawnAIActor(team, playerName, rot);
	APlaneAIController* controller = team.AIActors.Last()->GetController<APlaneAIController>();
	if (controller)
	{
		int index = FMath::RandRange(0, enemyTeamActors.Num() - 1);

		controller->SetTargetActor(enemyTeamActors[index]);
	}
}

void AAIManager::Respawn(FTeam& team, const TArray<TObjectPtr<class APlanePawnAI>>& enemyTeamActors, const FString& playerName, const FRotator& rot)
{
	for (int i = 0; i < team.AIActors.Num(); i++)
	{
		if (playerName == team.AIActors[i]->GetGameName())
		{
			team.AIActors.RemoveAt(i);
		}
	}

	if (team.TimeSinceLastSpawn > team.SpawnCooldown)
	{
		SpawnAIActorGameInProgress(team, enemyTeamActors, playerName);
	}
	else
	{
		// start a timer to spawn an actor
		FTimerDelegate Delegate; // Delegate to bind function with parameters
		Delegate.BindUFunction(this, "SpawnAIActorGameInProgress", team, enemyTeamActors, playerName, rot);
		float delay = team.SpawnCooldown - team.TimeSinceLastSpawn;
		GetWorld()->GetTimerManager().SetTimer(team.SpawnTimerHandler, Delegate, delay, false);

	}
}

