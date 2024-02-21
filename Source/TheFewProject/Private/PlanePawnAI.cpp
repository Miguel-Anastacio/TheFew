;// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanePawnAI.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"
#include "VFX/VfxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Physics/AircraftPhysics.h"
#include "Managers/AIManager.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/WidgetComponent.h"
#include "UI/IndicatorWidget.h"
#include "Game/ArenaGameState.h"
#include "PaperSpriteComponent.h"
#include "Player/PlanePawnPlayer.h"
APlanePawnAI::APlanePawnAI()
{
	DetectionVolume = CreateDefaultSubobject<UBoxComponent>("Detection Volume");
	DetectionVolume->SetupAttachment(PlaneBodyBox);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget");
	WidgetComponent->SetupAttachment(RootComponent);
}


void APlanePawnAI::BeginPlay()
{
	Super::BeginPlay();
	PlanePhysicsComponent->SetThrottleInput(1);
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
}

TObjectPtr<class UBoxComponent> APlanePawnAI::GetDetectionVolume()
{
	return DetectionVolume;
}

void APlanePawnAI::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!IsValid(OtherActor))
		return;

	if (OtherActor->ActorHasTag("Terrain"))
	{
		FVector current = GetActorLocation();
		current.Z = 10000.f;
		SetActorLocation(current);
		if (GetOwner())
		{
			AAIManager* mgr = Cast<AAIManager>(GetOwner());
			if(mgr)
				mgr->IncreaseCrashes();
		}
		PlaneDeath(OtherActor);

	}
}

// NOT IN USE
void APlanePawnAI::OnOverlapDetectionEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor))
		return;

	if (OtherActor->ActorHasTag("Bounds"))
	{
		//FVector current = GetActorLocation();
		//current.Z = 10000.f;
		//SetActorLocation(current);

		PlaneDeath(OtherActor);
		//Destroy();
	}
}

void APlanePawnAI::PlaneDeath(AActor* instigator)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BigExplosionEffect, GetActorLocation());
	//PlaneBodyBox->AddForce(FVector(0, 0, -100000.0f));
	//PlanePhysicsComponent->DestroyComponent();
	APlanePawn* other = Cast<APlanePawn>(instigator);
	if (!IsValid(other))
	{
		Destroy();
		return;
	}

	if (Cast<APlanePawnPlayer>(other))
	{
		int a = 0;
	}

	AArenaGameState* gameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
	if (!IsValid(gameState))
	{
		Destroy();
		return;
	}

	gameState->UpdateScoreboard(other->GetGameName(), this->GameName);
	Destroy();
}

void APlanePawnAI::SetWidgetColor(const FLinearColor& color)
{
	UUserWidget* widget = WidgetComponent->GetWidget();
	if (IsValid(widget))
	{
		widget->SetColorAndOpacity(color);
	}
	MinimapIcon->SetSpriteColor(color);

}

void APlanePawnAI::SetWidgetVisibility(ESlateVisibility visibiliy)
{
	UUserWidget* widget = WidgetComponent->GetWidget();
	if (IsValid(widget))
	{
		widget->SetVisibility(visibiliy);
	}
}

ESlateVisibility APlanePawnAI::GetWidgetVisibility()
{
	UUserWidget* widget = WidgetComponent->GetWidget();
	if (IsValid(widget))
	{
		return widget->GetVisibility();
	}
	return ESlateVisibility();
}
