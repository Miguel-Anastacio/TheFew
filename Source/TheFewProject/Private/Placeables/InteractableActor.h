// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ReactToClickInterface.h"
#include "InteractableActor.generated.h"

UCLASS()
class AInteractableActor : public AActor, public IReactToClickInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SetWidgetVisible();
	UFUNCTION()
	void HideWidget(const FVector& vec = FVector(0, 0, 0));
	UFUNCTION()
	void Disable();
	virtual void ReactToClick() override;
protected:

	UPROPERTY(EditAnywhere, Category = "Mesh")
		TObjectPtr<class UStaticMeshComponent> MeshComponent;
	UPROPERTY(EditAnywhere, Category = "UI")
		TObjectPtr<class  UWidgetComponent> WidgetComponent;



};
