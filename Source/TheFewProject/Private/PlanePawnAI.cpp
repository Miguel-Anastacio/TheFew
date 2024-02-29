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
#include "PlaneAIController.h"

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

//void APlanePawnAI::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	if (!IsValid(OtherActor))
//		return;
//
//	if (OtherActor->ActorHasTag("Terrain"))
//	{
//		if (GetOwner())
//		{
//			AAIManager* mgr = Cast<AAIManager>(GetOwner());
//			if(mgr)
//				mgr->IncreaseCrashes();
//		}
//		HealthComponent->TakeDamage(DamageTakenOnCrash, OtherActor);
//		//PlaneDeath(OtherActor);
//		return;
//	}
//	IReactToHitInterface* interface = Cast<IReactToHitInterface>(OtherActor);
//
//	if (interface)
//	{
//		if (interface->TeamID != TeamID)
//		{
//			HealthComponent->TakeDamage(DamageTakenOnCrash, OtherActor);
//		}
//
//		APlanePawn* plane = Cast<APlanePawn>(OtherActor);
//		if (plane && HealthComponent->IsAlive())
//		{
//			//UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTarget(this);
//			FVector awayFromOtherPlane = GetActorLocation() - plane->GetActorLocation();
//			awayFromOtherPlane.Normalize();
//			PlaneBodyBox->AddImpulse(awayFromOtherPlane * ForceAppliedOnCrash, NAME_None, true);
//
//
//		}
//	}
//}

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

void APlanePawnAI::ReactToHit(float damage, AActor* instigator)
{
	Super::ReactToHit(damage, instigator);
	APlanePawnPlayer* player = Cast<APlanePawnPlayer>(instigator);
	if (IsValid(player))
	{
		player->EnemyHitDelegate.Broadcast();
	}

}

void APlanePawnAI::PlaneDeath(AActor* instigator)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BigExplosionEffect, GetActorLocation());
	APlanePawn* other = Cast<APlanePawn>(instigator);
	if (!IsValid(other))
	{
		Destroy();
		return;
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

AActor* APlanePawnAI::GetTargetActor()
{
	APlaneAIController* controller = GetController<APlaneAIController>();
	if (controller)
	{
		return controller->GetTargetActor();
	}
	return nullptr;
}
