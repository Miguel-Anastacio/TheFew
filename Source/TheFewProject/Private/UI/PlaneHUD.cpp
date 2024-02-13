// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlaneHUD.h"
#include "PlanePawn.h"
#include "Physics/AircraftPhysics.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/HealthComponent.h"
void UPlaneHUD::SetPlaneReference(APlanePawn* ref)
{
	if (ref)
	{
		ControlledPlane = ref;
		ThrottleRef = ControlledPlane->GetPlanePhysicsComponent()->GetThrottleRef();

		UHealthComponent* comp = ControlledPlane->GetHealthComponent();
		comp->ActorDamageDelegate.AddDynamic(this, &UPlaneHUD::UpdateHealthBar);

		HealthBar->SetPercent(1);

	}
}

void UPlaneHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlaneHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (ControlledPlane)
	{
		ThrottleBar->SetPercent(*ThrottleRef);

		APlayerController* PC = GetOwningPlayer();
		FVector2D pivot;
		FVector loc = ControlledPlane->GetActorLocation() + ControlledPlane->GetActorForwardVector() * 20000.0f;

		PC->ProjectWorldLocationToScreen(loc, pivot);

		if (CrosshairWidget)
			CrosshairWidget->SetPositionInViewport(pivot);

		AltitudeValue->SetText(FText::AsNumber(int(ControlledPlane->GetActorLocation().Z * 0.01)));

		
	}
}

void UPlaneHUD::NativeOnInitialized()
{
	CrosshairWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), CrosshairImageClass);
	if (CrosshairWidget)
		CrosshairWidget->AddToViewport();
}

void UPlaneHUD::UpdateHealthBar(float currentPercent)
{
	HealthBar->SetPercent(currentPercent);
}
