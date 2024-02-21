// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDeathSiganture, AActor*, responsible);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorDeathSimpleSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDamageSignature, float, currentHealth);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE();

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void TakeDamage(float damage);
	void TakeDamage(float damage, AActor* causer);
	FORCEINLINE void SetMaxHealth(float health) { MaxHealth = health; };
	FORCEINLINE void Reset() { CurrentHealth = MaxHealth; Alive = true; };
	FORCEINLINE bool IsAlive() { return Alive; };
	//FORCEINLINE void AddHealth(float health) { CurrentHealth += health; };

public:
	UPROPERTY(BlueprintAssignable)
		FOnActorDeathSiganture ActorDeathDelegate;

	UPROPERTY(BlueprintAssignable)
		FOnActorDeathSimpleSignature ActorSimpleDeathDelegate;

	UPROPERTY(BlueprintAssignable)
		FOnActorDamageSignature ActorDamageDelegate;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.0f;

	float CurrentHealth = 100.0f;

	bool Alive = true;

		
};
