// Fill out your copyright notice in the Description page of Project Settings.

#include "VFX/VfxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/EngineTypes.h"
#include "../../TheFewProject.h"
// Sets default values for this component's properties
UVfxComponent::UVfxComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//TrailEffect = CreateDefaultSubObject<UNiagaraSystem>("Trail Effect");
	//TrailEffect->SetupAttachment(this);
	// ...
}


// Called when the game starts
void UVfxComponent::BeginPlay()
{
	Super::BeginPlay();
	TrailEffectInstance = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailEffect, this, "Trail", this->GetComponentLocation(), FRotator(0, 0, 0), EAttachLocation::KeepWorldPosition, true);
	if(TrailEffectInstance)
		TrailEffectInstance->SetNiagaraVariableFloat(FString("Lifetime"), 0.0f);
	//FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
	//bool result = TrailEffectInstance->AttachToComponent(this, rules);

	// ...
	
}


// Called every frame
void UVfxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	//if(TrailEffectInstance)
	//	TrailEffectInstance->SetNiagaraVariableFloat(FString("Lifetime"), CoefStrength);
	// ...
}

void UVfxComponent::UpdateNiagaraSystem(const FVector& velocity)
{
	if (TrailEffectInstance)
	{
		float currentSpeed = velocity.Size();
		float acceleration = currentSpeed - PreviousSpeed;
		acceleration = FMath::Clamp(acceleration, MinAcceleration, AccelerationMaxTrail);
		float lifetime = FMath::InterpEaseOut(0.f, MaxLifetime, (acceleration - MinAcceleration)/ (AccelerationMaxTrail - MinAcceleration), 4);
		//lifetime = 1.0f;
		if (lifetime < 0.1)
		{
			lifetime = 0;
			//TrailEffectInstance->Deactivate();
		}
		//else if(!TrailEffectInstance->IsActive())
		//{
		//	//TrailEffectInstance->Activate();
		//}

		//UE_LOG(LogProjectFew, Warning, TEXT("Acceleration %f"), acceleration);
		TrailEffectInstance->SetNiagaraVariableFloat(FString("Lifetime"), lifetime);

		PreviousSpeed = currentSpeed;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("NUl"));
	}
}

