#include "AIManager.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/AIManager.h"
#include "PlanePawnAI.h"
#include "PlaneAIController.h"
#include "PlaneController.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AAIManager::AAIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

	APlaneController* controller = Cast<APlaneController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	controller->InitDebugVariables(SpawnArea, this);

	if (SpawnArea)
	{
		FVector origin, extent;
		SpawnArea->GetActorBounds(false, origin, extent);
		SpawnAreaBoundsMax.X = origin.X + extent.X;
		SpawnAreaBoundsMax.Y = origin.Y + extent.Y;

		SpawnAreaBoundsMin.X = origin.X - extent.X;
		SpawnAreaBoundsMin.Y = origin.Y - extent.Y;

	}

	for (int i = 0; i < NumberOfEnemies; i++)
	{
		SpawnEnemy(SpawnAreaBoundsMin, SpawnAreaBoundsMax, SpawnHeight);
	}
}
// Called every frame
void AAIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlaneSelectedIndex > -1)
	{
		// DEBUG ONLY CODE
		APlaneAIController* controller = Cast<APlaneAIController>(Enemies[PlaneSelectedIndex]->Controller);
		if(controller)
			controller->ShowDebugInfo();
	}
}

void AAIManager::OnAIDestroyed(AActor* actor)
{
	//SpawnEnemy(SpawnAreaBoundsMin, SpawnAreaBoundsMax, SpawnHeight);
	//ChangePlaneSelected(1);

}

TObjectPtr<class APlanePawnAI> AAIManager::ChangePlaneSelected(float input)
{
	for (int i = 0; i < Enemies.Num(); i++)
	{
		if (!IsValid(Enemies[i]))
		{
			Enemies.RemoveAt(i);
		}
	}

	PlaneSelectedIndex += input;
	if (PlaneSelectedIndex >= Enemies.Num())
		PlaneSelectedIndex = 0;
	else if (PlaneSelectedIndex < 0)
		PlaneSelectedIndex = Enemies.Num() - 1;

	return Enemies[PlaneSelectedIndex];
}


void AAIManager::SpawnEnemy(const FVector2D& minBounds, const FVector2D& maxBounds, float zHeight)
{
	if (IsValid(EnemyClass))
	{
		FActorSpawnParameters params;
		FVector loc;
		loc.X = FMath::RandRange(minBounds.X, maxBounds.X);
		loc.Y = FMath::RandRange(minBounds.Y, maxBounds.Y);
		loc.Z = zHeight;
		FHitResult hit;
		GetWorld()->LineTraceSingleByChannel(hit, loc, loc + FVector::DownVector * 1000000, ECC_Visibility);
		int maxTries = 10;
		int counter = 0;
		while (!hit.bBlockingHit && counter < maxTries)
		{
			loc.X = FMath::RandRange(minBounds.X, maxBounds.X);
			loc.Y = FMath::RandRange(minBounds.Y, maxBounds.Y);
			GetWorld()->LineTraceSingleByChannel(hit, loc, loc + FVector::DownVector * 100000, ECC_Visibility);
			counter++;
		}

		if (counter > maxTries)
		{
			return;
		}

		TObjectPtr<APlanePawnAI> enemy = GetWorld()->SpawnActor<APlanePawnAI>(EnemyClass, loc, FRotator(0, 0, 0), params);
		if (IsValid(enemy))
		{
			enemy->SetOwner(this);
			enemy->OnDestroyed.AddDynamic(this, &AAIManager::OnAIDestroyed);
			UpdateTarget(enemy);
			Enemies.Add(enemy);
		}
	}
}

void AAIManager::UpdateTarget(AActor* actor)
{
	APlanePawnAI* enemy = Cast<APlanePawnAI>(actor);
	if (enemy)
	{
		APlaneAIController* controller = enemy->GetController<APlaneAIController>();
		if (controller)
		{
			int index = FMath::RandRange(0, Targets.Num() - 1);
			controller->SetTargetActor(Targets[index]);
		}
	}
}

