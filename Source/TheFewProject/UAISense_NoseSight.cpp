#include "UAISense_NoseSight.h"

// Sets default values
AUAISense_NoseSight::AUAISense_NoseSight()
{
	// Set this empty to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AUAISense_NoseSight::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUAISense_NoseSight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AUAISense_NoseSight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
