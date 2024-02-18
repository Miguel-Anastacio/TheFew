// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
//class UParticleSystemComponent;
UCLASS()
class AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetBulletVelocity(FVector dir);
	float GetBulletSpeed() { return BulletSpeed; };
	
	void SetTraceChannel(ECollisionChannel channel) { TraceChannelProperty = channel; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DrawTracer();

	void LineTrace();

protected:
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly)
		UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
		TObjectPtr<class UNiagaraSystem> ExplosionEffect;

	TObjectPtr<class UNiagaraComponent> TrailEffectInstance;

	UPROPERTY(EditDefaultsOnly)
		float BulletSpeed;
	UPROPERTY(EditDefaultsOnly)
		float BulletLifeTime = 10.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Tracer")
		float TracerLength = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Tracer")
		float TracerRenderTimer;
	UPROPERTY(EditDefaultsOnly, Category = "Tracer")
		float TracerThickness = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Collision")
		TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

	FTimerHandle LineTracerTimerHandle;
	FVector PreviousPosition = FVector(0, 0, 0);
	float LifeTime = 0.0;


};
