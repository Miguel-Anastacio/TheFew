// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlaneHUD.h"
#include "Player/PlanePawnPlayer.h"
#include "Physics/AircraftPhysics.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Components/RichTextBlock.h"
#include "Components/HealthComponent.h"
#include "UI/ScoreboardWidget.h"
#include "UI/DeathWidget.h"
#include "UI/OutOfBoundsWidget.h"
#include "UI/SpawnMenuWidget.h"
#include "UI/EndOfRoundWidget.h"
//#include "UI/TotalScoreWidget.h"
#include "Game/ArenaGameState.h"
#include "BattlePlaneGameMode.h"

void UPlaneHUD::NativeConstruct()
{
	Super::NativeConstruct();
}
void UPlaneHUD::NativeOnInitialized()
{
	CrosshairWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), CrosshairImageClass);
	if (CrosshairWidget)
	{
		CrosshairWidget->AddToViewport();
		CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	ABattlePlaneGameMode* gameMode = Cast< ABattlePlaneGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(gameMode))
	{
		gameMode->SpawnMenuStateDelegate.AddDynamic(this, &UPlaneHUD::DisplaySpawnScreen);
		gameMode->EndOfRoundStateDelegate.AddDynamic(this, &UPlaneHUD::DisplayEndOfRound);
	}
	AArenaGameState* gameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
	if (gameState)
	{
		/*TotalScoreWidget*/
		gameState->SetHUDScoreWidgetRef(TotalScoreWidget);

		//GameTimer = gameState->
		GameState = gameState;
	}
}

void UPlaneHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (ControlledPlane.Get())
	{
		ThrottleBar->SetPercent(*ThrottleRef);

		APlayerController* PC = GetOwningPlayer();
		FVector2D pivot;
		FVector loc = ControlledPlane->GetActorLocation() + ControlledPlane->GetActorForwardVector() * 20000.0f;

		PC->ProjectWorldLocationToScreen(loc, pivot);

		if (CrosshairWidget)
			CrosshairWidget->SetPositionInViewport(pivot);

		// Maybe it should be distance to the ground?
		AltitudeValue->SetText(FText::AsNumber(int(ControlledPlane->GetActorLocation().Z * 0.01)));
		int Velocity = ControlledPlane->GetVelocity().Size();
		VelocityValue->SetText(FText::AsNumber(Velocity));
	}

	if (GameState.Get())
	{
		float timer = GameState.Get()->GameTimer;
		int minutes = FMath::FloorToInt(timer / 60.f);
		int seconds = (int)timer % 60;

		if (MinutesText)
		{
			MinutesText->SetText(FText::AsNumber(minutes));
		}
		if (SecondsText)
		{
			SecondsText->SetText(FText::AsNumber(seconds));
		}
	}
}

void UPlaneHUD::SetPlaneReference(APlanePawnPlayer* ref)
{
	if (ref)
	{
		ControlledPlane = ref;
		ThrottleRef = ControlledPlane->GetPlanePhysicsComponent()->GetThrottleRef();

		UHealthComponent* comp = ControlledPlane->GetHealthComponent();
		comp->ActorDamageDelegate.AddDynamic(this, &UPlaneHUD::UpdateHealthBar);

		ControlledPlane->EnemyHitDelegate.AddDynamic(this, &UPlaneHUD::DisplayHitmarker);
		
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
			ScoreboardWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		ActiveWidget = ScoreboardWidget;
	}
	else
	{
		if(ScoreboardWidget)
			ScoreboardWidget->SetVisibility(ESlateVisibility::Collapsed);

		if (!IsValid(SpawnMenuWidget))
		{
			this->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

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
		if (OutOfBoundsWidget)
			OutOfBoundsWidget->SetVisibility(ESlateVisibility::Collapsed);
		if (ScoreboardWidget)
			ScoreboardWidget->SetVisibility(ESlateVisibility::Collapsed);

		this->SetVisibility(ESlateVisibility::Collapsed);
		CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
		DeathWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		//DeathWidget->AddToViewport();
		DeathWidget->ResetTimer();
		//FTimerHandle timer;
		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &UPlaneHUD::RemoveDeathScreen, DeathWidget->Lifetime, false);

		ActiveWidget = DeathWidget;
	}

}

void UPlaneHUD::DisplayOutOfBoundsWidget()
{
	if (!IsValid(OutOfBoundsWidget))
	{
		OutOfBoundsWidget = CreateWidget<UOutOfBoundsWidget>(GetOwningPlayer(), OutOfBoundsClass);
		OutOfBoundsWidget->AddToViewport();
	}
	if (OutOfBoundsWidget)
	{
		/*this->SetVisibility(ESlateVisibility::Collapsed);
		CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);*/
		OutOfBoundsWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		//DeathWidget->AddToViewport();
		OutOfBoundsWidget->ResetTimer();
		//FTimerHandle timer;
		//GetWorld()->GetTimerManager().SetTimer(timer, this, &UPlaneHUD::RemoveDeathScreen, DeathWidget->Lifetime, false);
		ActiveWidget = OutOfBoundsWidget;
	}
}

void UPlaneHUD::RemoveOutOfBoundsWidget()
{
	if (OutOfBoundsWidget && OutOfBoundsWidget->IsVisible())
	{
		OutOfBoundsWidget->PlayExitAnimation();
	}
}

float UPlaneHUD::GetLifetimeOfOutOfBoundsWidget()
{
	if (!IsValid(OutOfBoundsWidget))
	{
		OutOfBoundsWidget = CreateWidget<UOutOfBoundsWidget>(GetOwningPlayer(), OutOfBoundsClass);
		return OutOfBoundsWidget->Lifetime;
	}

	return OutOfBoundsWidget->Lifetime;
}

void UPlaneHUD::DisplaySpawnScreen()
{
	if (!IsValid(SpawnMenuWidget))
	{
		SpawnMenuWidget = CreateWidget<USpawnMenuWidget>(GetOwningPlayer(), SpawnClass);
		SpawnMenuWidget->AddToViewport();
		AArenaGameState* gameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
		if (gameState)
		{
			gameState->SetScoreboardSpawnWidgetRef(SpawnMenuWidget);
			FTeamGameData team = gameState->GetTeamAData();
			SpawnMenuWidget->InitTeamAData(team.TeamName, team.Kills, team.ID);
			team = gameState->GetTeamBData();
			SpawnMenuWidget->InitTeamBData(team.TeamName, team.Kills, team.ID);
		}
		//this->SetVisibility(ESlateVisibility::Collapsed);
		//CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
		if(DeathWidget)
			DeathWidget->SetVisibility(ESlateVisibility::Collapsed);

		ActiveWidget = SpawnMenuWidget;
	}
}

void UPlaneHUD::RemoveSpawnScreen()
{
	if (SpawnMenuWidget)
	{
		//SpawnMenuWidget->RemoveFromViewport();
		SpawnMenuWidget->RemoveFromParent();
		this->SetVisibility(ESlateVisibility::HitTestInvisible);
		CrosshairWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		SpawnMenuWidget = NULL;
	}
}


void UPlaneHUD::RemoveDeathScreen()
{
	if (DeathWidget)
	{
		//this->SetVisibility(ESlateVisibility::HitTestInvisible);
		//CrosshairWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		DeathWidget->SetVisibility(ESlateVisibility::Collapsed);

		ABattlePlaneGameMode* gameMode = Cast< ABattlePlaneGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (IsValid(gameMode))
		{
			gameMode->SpawnMenuStateDelegate.Broadcast();
		}
		//DeathWidget->RemoveFromParent();
	}
}

void UPlaneHUD::DisplayEndOfRound()
{
	ToggleScoreboard(true);
	if (!IsValid(EndOfRoundWidget))
	{
		EndOfRoundWidget = CreateWidget<UEndOfRoundWidget>(GetOwningPlayer(), EndOfRoundClass);
		EndOfRoundWidget->AddToViewport();
	}

	if (OutOfBoundsWidget)
		OutOfBoundsWidget->SetVisibility(ESlateVisibility::Collapsed);
	//if (ScoreboardWidget)
	//	ScoreboardWidget->SetVisibility(ESlateVisibility::Collapsed);
	if(DeathWidget)
		DeathWidget->SetVisibility(ESlateVisibility::Collapsed);

	if (SpawnMenuWidget)
		SpawnMenuWidget->SetVisibility(ESlateVisibility::HitTestInvisible);

	HideHitmarker();

	this->SetVisibility(ESlateVisibility::Collapsed);
	CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	ActiveWidget = EndOfRoundWidget;
}

void UPlaneHUD::UpdateHealthBar(float currentPercent)
{
	HealthBar->SetPercent(currentPercent);
}

void UPlaneHUD::FocusActiveWidget()
{
	if (ActiveWidget.Get())
		ActiveWidget->SetKeyboardFocus();
}

void UPlaneHUD::SetActiveWidget(UUserWidget* widget)
{
	ActiveWidget = widget;
}

void UPlaneHUD::DisplayHitmarker()
{
	if (HitmarkerIcon)
	{
		HitmarkerIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
		FTimerHandle timer;
		GetWorld()->GetTimerManager().SetTimer(timer, this, &UPlaneHUD::HideHitmarker, DurationHitmarker, false);
	}
}

void UPlaneHUD::HideHitmarker()
{
	if (HitmarkerIcon)
	{
		HitmarkerIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}