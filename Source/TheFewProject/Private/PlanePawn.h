// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Managers/AIManager.h"
#include "Interfaces/ReactToHitInterface.h"
#include "PlanePawn.generated.h"

class UBoxComponent;
class UCapsuleComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UAircraftPhysics;
class UWeaponComponent;
class UVfxComponent;
class UHealthComponent;
//class IReactToHitInterface;
UCLASS()
class APlanePawn : public APawn, public IReactToHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlanePawn();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	//float GetThrust() { return Thrust; };
	//void SetThrust(float t) { Thrust = t; };
	//void IncreaseThrust() { Thrust += 1; };

	void SwapCamera();
	void MoveCamera(FVector2D input);
	void ResetCameraLerpTimer() {
		rotTimer = 0;
	};
	void UpdateCamera(float DeltaTime);

	void ToggleLandingGear();
	UAircraftPhysics* GetPlanePhysicsComponent() { return PlanePhysicsComponent; };
	UHealthComponent* GetHealthComponent() { return HealthComponent; };
	UPrimitiveComponent* GetRigidbody();
	void TriggerWeapons();
	bool GetIsFlying() { return Flying; };

	void StopWeaponAudio();

	// override interface component functions
	/**A version of React To Trigger that can be implemented in C++ or Blueprint. */
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//void ReactToHit(float damage); virtual void ReactToHit_Implementation(float damage) override;
	void ReactToHit(float damage) override;
	void ReactToHit(float damage, AActor* instigator) override;

	FORCEINLINE void SetTeamID(int32 ID) { TeamID = ID; };
	FORCEINLINE const int32 GetTeamID() { return TeamID; };

	//FORCEINLINE void IncreaseKills() { GameData.Kills++; };

	FORCEINLINE void SetGameName(FString name){ GameName = name;};
	FORCEINLINE const FString GetGameName() { return GameName; };

	//void SetGameData(const FPlayerGameData& data);
	//FORCEINLINE FPlayerGameData GetGameData() { return GameData; };
	UFUNCTION()
		virtual void PlaneDeathSimple();

protected:
	virtual void BeginPlay() override;
	void CreateMeshWithPivot(USceneComponent* pivot, UStaticMeshComponent* mesh, FName namePivot, FName nameMesh);
	void AnimateControlSurface(float input, USceneComponent* surfacePivot, FRotator axis, float dt);
	void UpdateFlying();

	UFUNCTION()
		virtual void PlaneDeath(AActor* causer) {};
	UFUNCTION()
		virtual void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditDefaultsOnly)
	FString GameName;

	//UPROPERTY(VisibleAnywhere)
	//int32 TeamID;
	UPROPERTY(EditAnywhere)
		UBoxComponent* PlaneBodyBox;
	//UPROPERTY(EditAnywhere)
	//	TObjectPtr<UCapsuleComponent> PlaneWings;
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* BackWheel;
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* LeftWheelCollider;
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* RightWheelCollider;

	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* PlaneBodyMesh;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* PlanePropellerMesh;
	UPROPERTY(EditAnywhere)
		USceneComponent* PropellerRoot;

	UPROPERTY(EditAnywhere)
		USceneComponent* RudderRoot;
	UPROPERTY(EditAnywhere)
		USceneComponent* ElevatorRoot;
	UPROPERTY(EditAnywhere)
		USceneComponent* AileronLeftRoot;
	UPROPERTY(EditAnywhere)
		USceneComponent* AileronRightRoot;
	UPROPERTY(EditAnywhere)
		USceneComponent* LeftLandingGearRoot;
	UPROPERTY(EditAnywhere)
		USceneComponent* RightLandingGearRoot;

	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* AileronLeftMesh;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* AileronRightMesh;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* ElevatorMesh;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent*RudderMesh;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* LeftLandingGearMesh;
	UPROPERTY(EditAnywhere, Category = "Meshes")
		UStaticMeshComponent* RightLandingGearMesh;

	UPROPERTY(EditAnywhere, Category = "Tail Camera")
		USpringArmComponent* TailCameraBoom;
	UPROPERTY(EditAnywhere, Category = "Tail Camera")
		UCameraComponent* TailCamera;
	UPROPERTY(EditAnywhere, Category = "Propeller Camera")
		USpringArmComponent* PropellerCameraBoom;
	UPROPERTY(EditAnywhere, Category = "Propeller Camera")
		UCameraComponent* PropellerCamera;

	UPROPERTY(EditAnywhere, Category = "Physics")
		UAircraftPhysics* PlanePhysicsComponent;

	UPROPERTY(EditAnywhere, Category = "Weapon")
		UWeaponComponent* LeftWeaponComponent;
	UPROPERTY(EditAnywhere, Category = "Weapon")
		UWeaponComponent* RightWeaponComponent;

	UPROPERTY(EditAnywhere, Category = "Health")
		TObjectPtr<class UHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, Category = "Plane Animation")
		float PropellerRotationSpeed = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Plane Animation")
		float TargetAngle = 15.0f;
	UPROPERTY(EditAnywhere, Category = "Plane Animation")
		float ControlSurfacesAnimationSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Plane VFX")
		UVfxComponent* LeftTrail;
	UPROPERTY(EditAnywhere, Category = "Plane VFX")
		UVfxComponent* RightTrail;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float CameraMoveSpeed = 50.0f;
	// how much can the camera boom rotate in the Y and Z axis
	UPROPERTY(EditAnywhere, Category = "Camera")
		FVector2D MaxLookAngle = FVector2D(90, 180);
	UPROPERTY(EditAnywhere, Category = "Camera")
		float MaxFOV = 120.0f;
	UPROPERTY(EditAnywhere, Category = "Camera")
		float SpeedForMaxFOV = 3000.0f;
	float DefaultFOV = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Audio")
		UAudioComponent* PlaneEngineAudioComponent;
	UPROPERTY(EditAnywhere, Category = "Audio")
		UAudioComponent* GunFireAudioComponent;

	UPROPERTY(EditAnywhere, Category = "UI")
		TObjectPtr <USpringArmComponent> IconBoom;
	UPROPERTY(EditAnywhere, Category = "UI")
		TObjectPtr<class UPaperSpriteComponent> MinimapIcon;

	UPROPERTY(EditAnywhere, Category = "Landing Gear")
		float DeploySpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Landing Gear")
	bool LandingGear = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<class UNiagaraSystem> BigExplosionEffect;

	UPROPERTY(VisibleAnywhere)
	bool Flying = false;

	FVector2D CameraInput = FVector2D(0, 0);
	FRotator CameraRotDirection = FRotator(0, 0,0);
	FQuat TargetCameraRotation;
	float rotTimer = 0.0f;
	FRotator DefaultCameraRotation;

	FRotator LandingGearTargetRotation;
	
	friend class APlanePhysicsDebugHUD;
};
