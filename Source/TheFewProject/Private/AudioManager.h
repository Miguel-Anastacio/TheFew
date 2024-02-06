// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioManager.generated.h"

UCLASS()
class AAudioManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAudioManager();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere) UAudioComponent* PlaneEngineAudioComponent;
	UPROPERTY(VisibleAnywhere) UAudioComponent* CoinCollectedAudioComponent;

	UPROPERTY(EditAnywhere, Category = Sound)  TArray <class USoundCue*> SoundCues;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
