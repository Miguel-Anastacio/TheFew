// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Manager/ManagerHUD.h"
#include "Player/PlanePawnPlayer.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ScoreboardWidget.h"
#include "UI/DeathWidget.h"
#include "UI/PlaneHUD.h"
#include "UI/OutOfBoundsWidget.h"
#include "UI/SpawnMenuWidget.h"
#include "UI/EndOfRoundWidget.h"
#include "Game/ArenaGameState.h"
#include "BattlePlaneGameMode.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "LayerManagerHUD.h"
#include "LayerUI.h"
#include "UILayerManagerFunctionLibrary.h"
void AManagerHUD::Init()
{
	if (GameLayer)
	{
		ULayerUI* gameLayer = NewObject<ULayerUI>(GetTransientPackage(), GameLayer);
		RegisterLayer(Game, gameLayer);
	}
	if (PrioLayer)
	{
		ULayerUI* prioLayer = NewObject<ULayerUI>(GetTransientPackage(), PrioLayer);
		RegisterLayer(Priority, prioLayer);
	}


	if (!IsValid(GetWorld()))
	{
		return;
	}
	if (!IsValid(UGameplayStatics::GetGameMode(GetWorld())))
	{
		return;
	}
	ABattlePlaneGameMode* gameMode = Cast<ABattlePlaneGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(gameMode))
	{
		gameMode->SpawnMenuStateDelegate.AddDynamic(this, &AManagerHUD::DisplaySpawnScreen);
		gameMode->EndOfRoundStateDelegate.AddDynamic(this, &AManagerHUD::DisplayEndOfRound);
	}
}
void AManagerHUD::DisplayHUD()
{
	if (!IsValid(HUDWidget))
	{
		HUDWidget = CreateWidget<UPlaneHUD>(GetOwningPlayerController(), HUDClass);
		HUDWidget->AddToViewport();
		APlanePawnPlayer* player = Cast<APlanePawnPlayer>(GetOwningPawn());
		HUDWidget->SetPlaneReference(player);
	}

	PushToLayer(Game, HUDWidget);
}
void AManagerHUD::UpdateWidgetHUD(int32 healthBar)
{
	if (HUDWidget)
		HUDWidget->UpdateHealthBar(healthBar);
}
void AManagerHUD::ToggleScoreboard_Implementation(bool status)
{
	if (!IsValid(ScoreboardWidget))
	{
		ScoreboardWidget = CreateWidget<UScoreboardWidget>(GetOwningPlayerController(), ScoreboardClass);
		ScoreboardWidget->AddToViewport();
		//ActiveWidget = ScoreboardWidget;
		// this will be moved to the widget itself on pushed
		AArenaGameState* gameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
		if (gameState)
		{
			gameState->SetScoreboardWidgetRef(ScoreboardWidget);
			ScoreboardWidget->InitTeamA(gameState->GetTeamAData());
			ScoreboardWidget->InitTeamB(gameState->GetTeamBData());
		}
		PushToLayer(GameMenu, ScoreboardWidget);
		return;
	}

	if (ScoreboardWidget->IsVisible())
	{
		PopFromLayerWithFocus(GameMenu);
		// add functionality to peak the top of the layer
	}
	else if (status)
	{
		ScoreboardWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else 
	{
		PushToLayer(GameMenu, ScoreboardWidget);
		// this will be moved to the widget itself on pushed
		/*AArenaGameState* gameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
		if (gameState)
		{
			gameState->SetScoreboardWidgetRef(ScoreboardWidget);
			ScoreboardWidget->InitTeamA(gameState->GetTeamAData());
			ScoreboardWidget->InitTeamB(gameState->GetTeamBData());
		}*/
		//ActiveWidget = ScoreboardWidget;
	}
}

void AManagerHUD::TogglePause_Implementation()
{
	if (!IsValid(PauseMenuWidget))
	{
		PauseMenuWidget = CreateWidget<UEndOfRoundWidget>(GetOwningPlayerController(), PauseMenuClass);
		PauseMenuWidget->AddToViewport();
		ActiveWidget = PauseMenuWidget;
		PushToLayer(Priority, PauseMenuWidget);
		return;
	}

	ToggleWidget(PauseMenuWidget, Priority);
}
void AManagerHUD::ToggleWidget(UUserWidget* widget, const FString& layerName)
{
	if (widget->IsVisible())
	{
		PopFromLayerWithFocus(layerName);
		// add functionality to peak the top of the layer
	}
	else
	{
		PushToLayer(layerName, widget);
		ActiveWidget = widget;
	}
}

void AManagerHUD::DisplayDeathScreen()
{
	if (!IsValid(DeathWidget))
	{
		DeathWidget = CreateWidget<UDeathWidget>(GetOwningPlayerController(), DeathClass);
		DeathWidget->AddToViewport();
	}
	else
	{
		DeathWidget->ResetTimer();
	}

	PushToLayer(Priority, DeathWidget);
	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &AManagerHUD::RemoveDeathScreen, DeathWidget->Lifetime, false);
}

void AManagerHUD::RemoveDeathScreen()
{

	PopFromLayerWithFocus(Priority);
	DeathWidget = NULL;
	ABattlePlaneGameMode* gameMode = Cast<ABattlePlaneGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(gameMode))
	{
		gameMode->SpawnMenuStateDelegate.Broadcast();
	}
}

void AManagerHUD::DisplayOutOfBoundsWidget()
{
	if(!IsValid(OutOfBoundsWidget))
	{
		OutOfBoundsWidget = CreateWidget<UOutOfBoundsWidget>(GetOwningPlayerController(), OutOfBoundsClass);
		OutOfBoundsWidget->AddToViewport();
	}
	else
	{
		OutOfBoundsWidget->ResetTimer();
	}

	PushToLayer(Game, OutOfBoundsWidget);
	//ActiveWidget = OutOfBoundsWidget;

	//GetWorld()->GetTimerManager().SetTimer(OutOfBoundsHandle, this, &AManagerHUD::RemoveOutOfBoundsWidget, OutOfBoundsWidget->Lifetime, false);
}

void AManagerHUD::RemoveOutOfBoundsWidget()
{
	/*if (OutOfBoundsWidget && OutOfBoundsWidget->IsVisible())
	{
		OutOfBoundsWidget->PlayExitAnimation();
	}*/
	/*if (GetWorld()->GetTimerManager().IsTimerActive(OutOfBoundsHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(OutOfBoundsHandle);
	}*/
	if(OutOfBoundsWidget == PeakLayer(Game))
		PopFromLayer(Game);


}

float AManagerHUD::GetLifetimeOfOutOfBoundsWidget() const
{
	if (!IsValid(OutOfBoundsWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("Getting value from out of bounds widget before creating it"));
		return 0;
	}

	return OutOfBoundsWidget->Lifetime;
}

void AManagerHUD::DisplaySpawnScreen()
{
	if (!IsValid(SpawnMenuWidget))
	{
		SpawnMenuWidget = CreateWidget<USpawnMenuWidget>(GetOwningPlayerController(), SpawnClass);
		SpawnMenuWidget->AddToViewport();
		// Move this to the widget itself on pushed
		AArenaGameState* gameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
		if (gameState)
		{
			gameState->SetScoreboardSpawnWidgetRef(SpawnMenuWidget);
			FTeamGameData team = gameState->GetTeamAData();
			SpawnMenuWidget->InitTeamAData(team.TeamName, team.Kills, team.ID);
			team = gameState->GetTeamBData();
			SpawnMenuWidget->InitTeamBData(team.TeamName, team.Kills, team.ID);
		}
	}
	
	//GetLayer(Menu)->ClearStack();
	//while(PopFromLayer(Game))

	PushToLayer(Priority, SpawnMenuWidget);
	ActiveWidget = SpawnMenuWidget;
}
void AManagerHUD::DisplayEndOfRound()
{
	if (!IsValid(EndOfRoundWidget))
	{
		EndOfRoundWidget = CreateWidget<UEndOfRoundWidget>(GetOwningPlayerController(), EndOfRoundClass);
		EndOfRoundWidget->AddToViewport();

		// 
		/*AArenaGameState* gameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
		if (gameState)
		{
			gameState->SetScoreboardSpawnWidgetRef(SpawnMenuWidget);
			FTeamGameData team = gameState->GetTeamAData();
			SpawnMenuWidget->InitTeamAData(team.TeamName, team.Kills, team.ID);
			team = gameState->GetTeamBData();
			SpawnMenuWidget->InitTeamBData(team.TeamName, team.Kills, team.ID);
		}*/
	}

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	PushToLayer(Priority, EndOfRoundWidget);
	ActiveWidget = EndOfRoundWidget;

	// show scoredoard
	ToggleScoreboard(true);
}

void AManagerHUD::FocusActiveWidget() const
{
	if (ActiveWidget.Get())
		ActiveWidget->SetKeyboardFocus();
}

void AManagerHUD::UnFocusActiveWidget() const
{
	if (ActiveWidget.Get())
		UWidgetBlueprintLibrary::SetFocusToGameViewport();
}

void AManagerHUD::SetActiveWidget(UUserWidget* widget)
{
	ActiveWidget = widget;
}


void AManagerHUD::PopFromLayerWithFocus(const FString& layer)
{
	PopFromLayer(layer);
	// add functionality to peak the top of the layer
	const FString current = GetCurrentLayerTag();
	UUserWidget* top = PeakLayer(current);
	if (top)
	{
		ActiveWidget = top;
		FocusActiveWidget();
	}
}
