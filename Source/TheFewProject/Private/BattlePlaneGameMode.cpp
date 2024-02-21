// Fill out your copyright notice in the Description page of Project Settings.

#include "BattlePlaneGameMode.h"
#include "PlanePawn.h"

ABattlePlaneGameMode::ABattlePlaneGameMode()
{
	DefaultPawnClass = PlanePawnClass;
}

void ABattlePlaneGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ABattlePlaneGameMode::SwitchStateFunction(PlayerGameState state)
{
	/*switch (CurrentState)
	{
	case Playing : 
		break;
	case Main_Menu:
		break;
	case Spawn_Menu:
		break;
	case End_Of_Round:
		break;
	default:
		break;
	}*/

	/*switch (state)
	{
	case Playing:
		PlayingStateDelegate.Broadcast();
		break;
	case Spawn_Menu:
		SpawnMenuDelegate.Broadcast();
		break;
	case Main_Menu:
		break;
	case End_Of_Round:
		break;
	default:
		break;
	}*/
}
