// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VfxComponent.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UVfxComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVfxComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateNiagaraSystem(const FVector& velocity);
	void SetSpeedForMaxTrail(float speed) { SpeedForMaxTrail = speed; };
	//void SetVelocityRef(FVector* ref) { VelocityRef = ref; };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
		UNiagaraSystem* TrailEffect;

	
	UNiagaraComponent* TrailEffectInstance;
	float PreviousSpeed= 0;
	UPROPERTY(EditAnywhere, Category = "Controls")
	float SpeedForMaxTrail = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Controls")
	float AccelerationMaxTrail = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Controls")
		float MinAcceleration = 100.0f;
		
	UPROPERTY(EditAnywhere, Category = "Controls")
		float MaxLifetime = 0.8f;
};
