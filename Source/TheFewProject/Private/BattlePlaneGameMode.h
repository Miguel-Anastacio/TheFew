// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattlePlaneGameMode.generated.h"

UENUM(BlueprintType)
enum PlayerGameState
{
	Playing UMETA(DisplayName = "Playing"),
	Spawn_Menu   UMETA(DisplayName = "Spawn Menu"),
	Main_Menu      UMETA(DisplayName = "Main Menu"),
	End_Of_Round      UMETA(DisplayName = "End_Of_Round")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangeSignature, PlayerGameState, newState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerStateChangeSignatureSimple);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayingStartedSignature, const FVector&, location);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, PlayerGameState, newState);
/**
 * 
 */
class APlanePawn;
UCLASS()
class ABattlePlaneGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABattlePlaneGameMode();
	FOnPlayerStateChangeSignature PlayerStateDelegate;

	FOnPlayerStateChangeSignatureSimple SpawnMenuStateDelegate;
	FOnPlayingStartedSignature PlayingStateDelegate;
	FOnPlayerStateChangeSignatureSimple MainMenuStateDelegate;
	FOnPlayerStateChangeSignatureSimple EndOfRoundStateDelegate;
protected:
	void PostInitializeComponents() override;
	void BeginPlay() override;
	UFUNCTION()
		void SwitchStateFunction(PlayerGameState state);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Pawn")
		TSubclassOf<class APlanePawn> PlanePawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
		TEnumAsByte<PlayerGameState> CurrentState;



	
};
