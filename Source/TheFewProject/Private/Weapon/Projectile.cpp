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

	PreviousPosition = GetActorLocation();
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);

	// If the trace hit something, bBlockingHit will be true,
	// and its fields will be filled with detailed info about what was hit
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, Hit.Location);
		IReactToHitInterface* interface = Cast<IReactToHitInterface>(Hit.GetActor());

		if (interface)
		{
			IReactToHitInterface* ownerInterface = Cast<IReactToHitInterface>(GetOwner());
			if (ownerInterface && interface->TeamID == ownerInterface->TeamID)
			{
				Destroy();
				return;
			}

			UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *Hit.GetActor()->GetName());
			interface->ReactToHit(BulletDamage, GetOwner());

			Destroy();
		}
	}


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
	AActor* plane = GetOwner();
	CapsuleComponent->SetPhysicsLinearVelocity(dir * BulletSpeed + plane->GetVelocity());
}

