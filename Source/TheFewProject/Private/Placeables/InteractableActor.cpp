// Fill out your copyright notice in the Description page of Project Settings.


#include "Placeables/InteractableActor.h"
#include "BattlePlaneGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("UI");
	MeshComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	ABattlePlaneGameMode* gameMode = Cast<ABattlePlaneGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode)
	{
		gameMode->SpawnMenuStateDelegate.AddDynamic(this, &AInteractableActor::SetWidgetVisible);
		gameMode->PlayingStateDelegate.AddDynamic(this, &AInteractableActor::HideWidget);
		gameMode->EndOfRoundStateDelegate.AddDynamic(this, &AInteractableActor::Disable);
	}
	
}

void AInteractableActor::SetWidgetVisible()
{
	if(WidgetComponent)
		WidgetComponent->SetVisibility(true);
}

void AInteractableActor::HideWidget(const FVector& vec)
{
	if (WidgetComponent)
		WidgetComponent->SetVisibility(false);
}

void AInteractableActor::Disable()
{
	SetActorHiddenInGame(true);
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActor::ReactToClick()
{
	ABattlePlaneGameMode* gameMode = Cast<ABattlePlaneGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(gameMode))
	{
		gameMode->PlayingStateDelegate.Broadcast(GetActorLocation());
		//if (WidgetComponent)
		//	WidgetComponent->SetVisibility(false);
	}
}

