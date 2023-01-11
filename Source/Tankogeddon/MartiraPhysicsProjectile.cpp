// Fill out your copyright notice in the Description page of Project Settings.


#include "MartiraPhysicsProjectile.h"

#include "Particles/ParticleSystemComponent.h"

#include "TankPlayerController.h"
#include "TankPawn.h"
#include "TurretPawn.h"

#include "PhysicsComponent.h"

void AMartiraPhysicsProjectile::Start(UArrowComponent* SpawnPoint, float Range)
{
	// Активируемся
	SetActive(true);

	// новые координаты 
	SetActorLocation(SpawnPoint->GetComponentLocation());
	SetActorRotation(SpawnPoint->GetComponentRotation());

	FVector PlayerLocation;
	ATankPlayerController* TPC = Cast<ATankPlayerController>(GetWorld()->GetFirstPlayerController());
	if (TPC)
	{
		ATankPawn* Player = TPC->TankPawn;
		if (Player)
		{
			FTransform PlayerTransform = Player->ActorToWorld();

			PlayerLocation = PlayerTransform.GetLocation();

			UE_LOG(LogTemp, Warning, TEXT("PlayerLocation value is: %s"), *PlayerLocation.ToString());
		}
	}

	CurrentTrajectory = PhysicsComponent->GenerateTrajectoryBezie2P(
		GetActorLocation(),
		PlayerLocation,
		TrajectorySimulationMaxTime,
		TrajectorySimulationTimeStep
	);

	if (ShowTrajectory)
	{
		for (FVector position : CurrentTrajectory)
		{
			DrawDebugSphere(GetWorld(), position, 5, 8, FColor::Purple, true, 1, 0, 2);
		}
	}

	TragectoryPointIndex = 0;


	// включаем видимость
	Mesh->SetVisibility(true);

	// отключаем видимость
	TrailEffect->SetVisibility(false);

	// включаем колизию
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// вычисляем время жизни
	SetTimeLive(Range);

	// запускаем таймер жизни
	//GetWorld()->GetTimerManager().SetTimer(LiveTimerHandle, this, &AProjectile::ReturnPool, TimeLive, false);

	// поехали!
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AMartiraPhysicsProjectile::Move, MoveRate, true, MoveRate);
}

void AMartiraPhysicsProjectile::Move()
{
	APhysicsProjectile::Move();
}
