// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlaneHUD.h"
#include "PlanePawn.h"
#include "Physics/AircraftPhysics.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
void UPlaneHUD::SetPlaneReference(APlanePawn* ref)
{
	ControlledPlane = ref;
	ThrottleRef = ControlledPlane->GetPlanePhysicsComponent()->GetThrottleRef();
}

void UPlaneHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlaneHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	ThrottleBar->SetPercent(*ThrottleRef);

	APlayerController* PC = GetOwningPlayer();
	FVector2D pivot;
	FVector loc = ControlledPlane->GetActorLocation();

	PC->ProjectWorldLocationToScreen(loc, pivot);

	if (!CrosshairWidget)
	{
		CrosshairWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), CrosshairImageClass);
		if(CrosshairWidget)
			CrosshairWidget->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("Crosshair"));
	}

	UGameplayStatics::ProjectWorldToScreen(
		GetOwningPlayer(),
		loc + ControlledPlane->GetActorForwardVector() * 2000.0f,
		pivot,
		true
	);
	//UCanvasPanelSlot* ImageAsPanelSlot = Cast<UCanvasPanelSlot>(CrosshairImage->Slot);
	////ImageAsPanelSlot->SetDesiredPosition(FVector2D(10000, 500));
	////ThrottleBar
	//pivot *= FMath::Pow(UWidgetLayoutLibrary::GetViewportScale(this), -1);
	////pivot *= FMath::Pow(UWidgetLayoutLibrary::GetViewportScale(this), -1);
	//FWidgetTransform transform = FWidgetTransform(pivot, FVector2D(1.0f, 1.0), FVector2D(0.0f, 0.0f), 0);
	//CrosshairImage->SetRenderTransform(transform);
	if (CrosshairWidget)
		CrosshairWidget->SetPositionInViewport(pivot);
}

//void UPlaneHUD::NativeOnInitialized()
//{
//	CrosshairWidget = CreateWidget<UPlaneHUD>(GetOwningPlayer(), CrosshairImageClass);
//	CrosshairWidget->AddToViewport();
//}
