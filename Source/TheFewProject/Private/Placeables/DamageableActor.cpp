// Fill out your copyright notice in the Description page of Project Settings.


#include "Placeables/DamageableActor.h"
#include "DamageableActor.h"
#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Managers/AIManager.h"
#include "PlaneAIController.h"
// Sets default values
ADamageableActor::ADamageableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Indicator");
	MeshComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void ADamageableActor::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(HealthComponent))
	{
		HealthComponent->ActorDeathDelegate.AddDynamic(this, &ADamageableActor::OnActorDeath);
	}

	if (IsValid(GetOwner()))
	{
		Manager = Cast<AAIManager>(GetOwner());
	}
}

// Called every frame
void ADamageableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamageableActor::ReactToHit(float damage)
{
	HealthComponent->TakeDamage(damage);
}

void ADamageableActor::ReactToHit(float damage, AActor* responsible)
{
	HealthComponent->TakeDamage(damage, responsible);
}

void ADamageableActor::OnActorDeath(AActor* causer)
{
	// get manager
	// manager assigns the causer a new target
	HealthComponent->Reset();
	if (IsValid(Manager))
	{
		Manager->UpdateTarget(causer);
		Manager->IncreaseTargetsDestroyed();
	}
}

