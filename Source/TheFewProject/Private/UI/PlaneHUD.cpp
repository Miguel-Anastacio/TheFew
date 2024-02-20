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
#include "UI/ScoreboardWidget.h"
#include "UI/DeathWidget.h"
#include "Game/ArenaGameState.h"
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

void UPlaneHUD::ToggleScoreboard(bool status)
{
	if (status)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);

		if (!IsValid(ScoreboardWidget))
		{
			ScoreboardWidget = CreateWidget<UScoreboardWidget>(GetOwningPlayer(), ScoreboardClass);
			if (ScoreboardWidget)
			{
				ScoreboardWidget->AddToViewport();
				AArenaGameState* gameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
				if (gameState)
				{
					gameState->SetScoreboardWidgetRef(ScoreboardWidget);
					ScoreboardWidget->InitTeamA(gameState->GetTeamAData());
					ScoreboardWidget->InitTeamB(gameState->GetTeamBData());
				}
			}
		}
		else
		{
			ScoreboardWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if(ScoreboardWidget)
			ScoreboardWidget->SetVisibility(ESlateVisibility::Collapsed);

		this->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPlaneHUD::DisplayDeathScreen()
{
	if (!IsValid(DeathWidget))
	{
		DeathWidget = CreateWidget<UDeathWidget>(GetOwningPlayer(), DeathClass);
		DeathWidget->AddToViewport();
	}
	if (DeathWidget)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
		DeathWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		DeathWidget->ResetTimer();
		FTimerHandle timer;
		GetWorld()->GetTimerManager().SetTimer(timer, this, &UPlaneHUD::RemoveDeathScreen, DeathWidget->Lifetime, false);
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

void UPlaneHUD::RemoveDeathScreen()
{
	if (DeathWidget)
	{
		this->SetVisibility(ESlateVisibility::HitTestInvisible);
		CrosshairWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		DeathWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPlaneHUD::UpdateHealthBar(float currentPercent)
{
	HealthBar->SetPercent(currentPercent);
}
