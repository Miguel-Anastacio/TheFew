// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "HealthComponent.h"
#include "../../TheFewProject.h"
// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHealthComponent::TakeDamage(float damage)
{
	CurrentHealth -= damage;
	//GetOwner()->OnTakeAnyDamage.Broadcast();
	ActorDamageDelegate.Broadcast(CurrentHealth/MaxHealth);
	if (CurrentHealth <= 0)
	{
		ActorSimpleDeathDelegate.Broadcast();
	}
	UE_LOG(LogProjectFew, Warning, TEXT("Current health of %s : %f"), *GetOwner()->GetName(), CurrentHealth);
}

void UHealthComponent::TakeDamage(float damage, AActor* causer)
{
	CurrentHealth -= damage;
	ActorDamageDelegate.Broadcast(CurrentHealth/MaxHealth);
	if (CurrentHealth <= 0 && Alive)
	{
		Alive = false;
		ActorDeathDelegate.Broadcast(causer);
	}
	UE_LOG(LogProjectFew, Warning, TEXT("Current health of %s : %f"), *GetOwner()->GetName(), CurrentHealth);
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

