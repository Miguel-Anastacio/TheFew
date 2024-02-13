// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ReactToHitInterface.h"
#include "DamageableActor.generated.h"
UCLASS()
class ADamageableActor : public AActor, public IReactToHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageableActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Health")
		TObjectPtr<class UHealthComponent> HealthComponent;
	UPROPERTY(EditAnywhere, Category = "Mesh")
		TObjectPtr<class UStaticMeshComponent> MeshComponent;
	UPROPERTY(EditAnywhere, Category = "UI")
		TObjectPtr<class UWidgetComponent> WidgetComponent;


	void ReactToHit(float damage) override;
	void ReactToHit(float damage, AActor* responsible) override;

	TObjectPtr<class AAIManager> Manager;
	UFUNCTION()
		void OnActorDeath(AActor* causer);
};
