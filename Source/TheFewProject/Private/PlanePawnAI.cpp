;// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanePawnAI.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "VFX/VfxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Physics/AircraftPhysics.h"
#include "Managers/AIManager.h"

APlanePawnAI::APlanePawnAI()
{
	DetectionVolume = CreateDefaultSubobject<UBoxComponent>("Detection Volume");
	DetectionVolume->SetupAttachment(PlaneBodyBox);
}
void APlanePawnAI::BeginPlay()
{
	Super::BeginPlay();
	PlaneBodyBox->OnComponentHit.AddDynamic(this, &APlanePawnAI::OnCompHit);
	PlaneBodyBox->OnComponentEndOverlap.AddDynamic(this, &APlanePawnAI::OnOverlapEnd);

	//PlaneBodyBox->SetPhysicsLinearVelocity(FVector(600.0f, 0, 0));
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

	LeftTrail->Deactivate();
	RightTrail->Deactivate();
	//LeftTrail->DestroyComponent();
	//RightTrail->DestroyComponent();
}
void APlanePawnAI::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	//FVector dirVel = PlaneBodyBox->GetPhysicsLinearVelocity();
	//FRotator faceRot = dirVel.Rotation();
	//
	//FVector input = PlanePhysicsComponent->GetControlInput();
	//FVector boxDirection = UKismetMathLibrary::RotateAngleAxis(GetActorForwardVector(), 60 * input.Y, FVector::RightVector);

	//faceRot = UKismetMathLibrary::FindLookAtRotation(DetectionVolume->GetComponentLocation(), DetectionVolume->GetComponentLocation() + boxDirection * 100.f);
	//faceRot = FQuat::Slerp(DetectionVolume->GetRelativeRotation().Quaternion(), faceRot.Quaternion(), deltaTime).Rotator();
	//DetectionVolume->SetRelativeRotation(faceRot);
	// Display Plane Data
}

TObjectPtr<class UBoxComponent> APlanePawnAI::GetDetectionVolume()
{
	return DetectionVolume;
}
void APlanePawnAI::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Terrain"))
	{
		FVector current = GetActorLocation();
		current.Z = 10000.f;
		SetActorLocation(current);
		if (GetOwner())
		{
			AAIManager* mgr = Cast<AAIManager>(GetOwner());
			mgr->IncreaseCrashes();
		}
		//Destroy();
	}
}

void APlanePawnAI::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Bounds"))
	{
		FVector current = GetActorLocation();
		current.Z = 10000.f;
		SetActorLocation(current);
		//Destroy();
	}
}
