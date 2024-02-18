// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponComponent.h"
#include "Weapon/Projectile.h"
#include "../../TheFewProject.h"
#include "Interfaces/ReactToHitInterface.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots = 60 / RateOfFire;
	DistanceFromNose = FMath::Max(100, DistanceFromNose);
	// ...
	
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Timer += DeltaTime;
	// ...
}

void UWeaponComponent::FireBullet()
{
	if (Timer < TimeBetweenShots)
		return;

	Timer = 0.0f;

	FActorSpawnParameters params;
	//params.
	FVector loc = this->GetComponentLocation();
	AProjectile* bullet = GetWorld()->SpawnActor<AProjectile>(BulletTemplate, loc, FRotator(0, 0, 0), params);
	bullet->SetOwner(this->GetOwner());
	AActor* plane = GetOwner();

	//IReactToHitInterface* interface = Cast<IReactToHitInterface>(plane);
	//if (interface)
	//{
	//	//UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *Hit.GetActor()->GetName());
	//	interface->ReactToHit(200.0f, GetOwner());
	//}

	FVector dir = plane->GetActorForwardVector() * DistanceFromNose + plane->GetActorLocation() - GetComponentLocation();
	dir.Normalize();
	BulletManager.Add(bullet);
	//dir = GetOwner()->GetActorForwardVector();
	FVector perpendicular = plane->GetActorForwardVector().Cross(FVector::UpVector);
	FVector dirHorizontal = FVector::VectorPlaneProject(dir, perpendicular);
	//dir = plane->GetActorForwardVector() * DistanceFromNose;
	//dir.Normalize();
	//UE_LOG(LogProjectFew, Warning, TEXT("Fire direction: X=%f, Y=%f, Z=%f"), dirHorizontal.X, dirHorizontal.Y, dirHorizontal.Z);


	FVector end = dir * bullet->GetBulletSpeed() + GetComponentLocation();
	bullet->SetBulletVelocity(dir);
	//DrawDebugLine(GetWorld(), GetComponentLocation(), end, FColor::Green, false, 0.1f, 0, 1.0f);
}

