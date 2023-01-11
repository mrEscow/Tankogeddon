// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"

#include "DrawDebugHelpers.h"
#include "PhysicsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

#include "TankPlayerController.h"

#include "TankPawn.h"
#include "TurretPawn.h"


APhysicsProjectile::APhysicsProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent->SetMobility(EComponentMobility::Movable);

	// отключаем видимость
	TrailEffect->SetVisibility(false);

	PhysicsComponent = CreateDefaultSubobject<UPhysicsComponent>(TEXT("PhysicsComponent"));

}

void APhysicsProjectile::Start(UArrowComponent* SpawnPoint, float Range)
{
	// Активируемся
	SetActive(true);

	// новые координаты 
	SetActorLocation(SpawnPoint->GetComponentLocation());
	SetActorRotation(SpawnPoint->GetComponentRotation());

	FVector2D ViewportSize2D;
	FVector2D MousePos2D;

	GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize2D);
	GetWorld()->GetGameViewport()->GetMousePosition(MousePos2D);

	//UE_LOG(LogTemp, Warning, TEXT("ViewportSize2D value is: %s"), *ViewportSize2D.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("MousePos2D     value is: %s"), *MousePos2D.ToString());

	float DistanceToMouse = FVector2D::Distance(ViewportSize2D / 2, MousePos2D);

	//UE_LOG(LogTemp, Warning, TEXT("DistanceToMouse: %f. "), DistanceToMouse);


	FVector forward = GetActorForwardVector();  // направление

	MoveVector =  forward * DistanceToMouse * TrajectorySimulationSpeed; // TrajectorySimulationSpeed = 1.5

	float g = 9.8f;

	float SightAngle = 0.5 * FMath::Asin(DistanceToMouse * g / (TrajectorySimulationSpeed * TrajectorySimulationSpeed));

	//MoveVector.Z = SightAngle;

	UE_LOG(LogTemp, Warning, TEXT("SightAngle  value is: %f"), SightAngle);

	CurrentTrajectory = PhysicsComponent->GenerateTrajectory(

		GetActorLocation(), 

		MoveVector, 

		TrajectorySimulationMaxTime, 

		TrajectorySimulationTimeStep, 

		0
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
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &APhysicsProjectile::Move, MoveRate, true, MoveRate);
}

void APhysicsProjectile::Move()
{
	FVector currentMoveVector = CurrentTrajectory[TragectoryPointIndex] - GetActorLocation();
	currentMoveVector.Normalize();

	FVector newLocation = GetActorLocation() + currentMoveVector * MoveSpeed * MoveRate;

	SetActorLocation(newLocation);

	if (FVector::Distance(newLocation, CurrentTrajectory[TragectoryPointIndex]) <= MoveAccurency)
	{
		TragectoryPointIndex++;

		if (TragectoryPointIndex >= CurrentTrajectory.Num())
		{
			// отключаем видимость
			TrailEffect->SetVisibility(false);

			Explode();

			ReturnPool();
		}
		else
		{
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TragectoryPointIndex]);
			SetActorRotation(newRotation);
		}
	}
}