// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile.h"
#include "../../TheFewProject.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interfaces/ReactToHitInterface.h"
#include "NiagaraFunctionLibrary.h"
//#include "Particles/ParticleSystemComponent.h"
// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Collision Capsule");
	RootComponent = CapsuleComponent;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Bullet Mesh");
	Mesh->SetupAttachment(RootComponent);
	CapsuleComponent->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	//CapsuleComponent->SetPhysicsLinearVelocity(BulletSpeed * GetActorForwardVector());
	//CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
	GetWorld()->GetTimerManager().SetTimer(
		LineTracerTimerHandle,
		this,
		&AProjectile::DrawTracer,
		TracerRenderTimer,
		true
	);

	PreviousPosition = GetActorLocation();
}

void AProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(LineTracerTimerHandle);
}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogProjectFew, Log, TEXT("Hit Actor"));
}

void AProjectile::DrawTracer()
{
	FVector start = GetActorLocation();
	FVector vel = CapsuleComponent->GetPhysicsLinearVelocity();
	vel.Normalize();
	FVector end = start + vel * TracerLength;
	DrawDebugLine(GetWorld(), start, end, FColor::Yellow,false, 0.05f, 0, TracerThickness);
	//UE_LOG(LogProjectFew, Log, TEXT("Draw Trace"));
}

void AProjectile::LineTrace()
{
	// FHitResult will hold all data returned by our line collision query
	FHitResult Hit;

	// We set up a line trace from our current location to a point 1000cm ahead of us
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = PreviousPosition;

	// You can use FCollisionQueryParams to further configure the query
	// Here we add ourselves to the ignored list so we won't block the trace
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());

	// To run the query, you need a pointer to the current level, which you can get from an Actor with GetWorld()
	// UWorld()->LineTraceSingleByChannel runs a line trace and returns the first actor hit over the provided collision channel.
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);


	//// You can use DrawDebug helpers and the log to help visualize and debug your trace queries.
	////DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
	//UE_LOG(LogTemp, Log, TEXT("Tracing line: %s to %s"), *TraceStart.ToCompactString(), *TraceEnd.ToCompactString());

	// If the trace hit something, bBlockingHit will be true,
	// and its fields will be filled with detailed info about what was hit
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		/*APlanePawn* pawn = Cast<APlanePawn>(Hit.GetActor());
		if (pawn)
		{
			pawn->gethealthco
		}*/

		//IReactToTriggerInterface* ReactingObjectA = Cast<IReactToTriggerInterface>(Hit.GetActor());

		/*if (Hit.GetActor()->GetClass()->ImplementsInterface(UReactToHitInterface::StaticClass()))
		{
			IReactToHitInterface::Execute_ReactToHit(Hit.GetActor(), 20.0f);
		}*/

		IReactToHitInterface* interface = Cast<IReactToHitInterface>(Hit.GetActor());
		if (interface)
		{
			UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *Hit.GetActor()->GetName());
			interface->ReactToHit(200.0f, GetOwner());
		}
		
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, Hit.Location);
		Destroy();
		/*if (pointerToAnyUObject->GetClass()->ImplementsInterface(UReactsToTimeOfDay::StaticClass())) {
			IReactsToTimeOfDay::Execute_ReactToHighNoon(pointerToAnyUObject);
		}*/

		/*if (MyObject->GetClass()->ImplementsInterface(UReactToTriggerInterface::StaticClass()))
			IMyInterface::Execute_MyFunction(MyObject, MyArg);*/
	}
	else {
		//UE_LOG(LogTemp, Log, TEXT("No Actors were hit"));
	}

	PreviousPosition = GetActorLocation();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LineTrace();
	LifeTime += DeltaTime;
	if (LifeTime > BulletLifeTime)
		Destroy();
}

void AProjectile::SetBulletVelocity(FVector dir)
{
	CapsuleComponent->SetPhysicsLinearVelocity(dir * BulletSpeed);
}

