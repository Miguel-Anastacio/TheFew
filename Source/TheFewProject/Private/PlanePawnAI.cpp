;// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanePawnAI.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "VFX/VfxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Physics/AircraftPhysics.h"

APlanePawnAI::APlanePawnAI()
{
	DetectionVolume = CreateDefaultSubobject<UBoxComponent>("Detection Volume");
	DetectionVolume->SetupAttachment(PlaneBodyBox);
}
TObjectPtr<class UBoxComponent> APlanePawnAI::GetDetectionVolume()
{
	return DetectionVolume;
}
void APlanePawnAI::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	FVector dirVel = PlaneBodyBox->GetPhysicsLinearVelocity();
	FRotator faceRot = dirVel.Rotation();
	
	FVector input = PlanePhysicsComponent->GetControlInput();
	FVector boxDirection = UKismetMathLibrary::RotateAngleAxis(GetActorForwardVector(), 60 * input.Y, FVector::RightVector);

	faceRot = UKismetMathLibrary::FindLookAtRotation(DetectionVolume->GetComponentLocation(), DetectionVolume->GetComponentLocation() + boxDirection * 100.f);
	FQuat::Slerp(DetectionVolume->GetRelativeRotation().Quaternion(), faceRot.Quaternion(), deltaTime);
	//DetectionVolume->SetRelativeRotation(faceRot);
	// Display Plane Data
}
void APlanePawnAI::BeginPlay()
{
	Super::BeginPlay();

	//UPROPERTY(EditAnywhere, Category = "Tail Camera")
	//	USpringArmComponent* TailCameraBoom;
	//UPROPERTY(EditAnywhere, Category = "Tail Camera")
	//	UCameraComponent* TailCamera;
	//UPROPERTY(EditAnywhere, Category = "Propeller Camera")
	//	USpringArmComponent* PropellerCameraBoom;
	//UPROPERTY(EditAnywhere, Category = "Propeller Camera")
	//	UCameraComponent* PropellerCamera;

	//UPROPERTY(EditAnywhere, Category = "Plane VFX")
	//	UVfxComponent* LeftTrail;
	//UPROPERTY(EditAnywhere, Category = "Plane VVFX")
	//	UVfxComponent* RightTrail;
	//TailCamera->DestroyComponent();
	//TailCameraBoom->DestroyComponent();

	//PropellerCamera->DestroyComponent();
	//PropellerCameraBoom->DestroyComponent();

	//LeftTrail->DestroyComponent();
	//RightTrail->DestroyComponent();
}
