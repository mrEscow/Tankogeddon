// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"

#include "DrawDebugHelpers.h"
#include "PhysicsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

#include "TankPlayerController.h"


APhysicsProjectile::APhysicsProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent->SetMobility(EComponentMobility::Movable);

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Traileffect"));
	TrailEffect->SetMobility(EComponentMobility::Movable);
	TrailEffect->SetupAttachment(RootComponent);

	PhysicsComponent = CreateDefaultSubobject<UPhysicsComponent>(TEXT("PhysicsComponent"));

}

void APhysicsProjectile::Start(UArrowComponent* SpawnPoint, float Range)
{
	// Активируемся
	SetActive(true);

	// новые координаты 
	SetActorLocation(SpawnPoint->GetComponentLocation());
	SetActorRotation(SpawnPoint->GetComponentRotation());


	//MoveVector = GetActorForwardVector() * TrajectorySimulationSpeed;

	//MoveVector = MoveVector + GetActorForwardVector() * TrajectorySimulationSpeed;

	FVector2D ViewportSize2D;
	FVector2D MousePos2D;

	GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize2D);
	GetWorld()->GetGameViewport()->GetMousePosition(MousePos2D);

	UE_LOG(LogTemp, Warning, TEXT("ViewportSize2D value is: %s"), *ViewportSize2D.ToString());
	UE_LOG(LogTemp, Warning, TEXT("MousePos2D     value is: %s"), *MousePos2D.ToString());

	float DistanceToMouse = FVector2D::Distance(ViewportSize2D / 2, MousePos2D);

	UE_LOG(LogTemp, Warning, TEXT("DistanceToMouse: %f. "), DistanceToMouse);


	FVector forward = GetActorForwardVector();  // направление

	//FVector MousePos;
	//FRotator currentRotation;
	//FVector MouseDirection;
	//DeprojectMousePositionToWorld(MousePos, MouseDirection);

	//AActor* owner = GetOwner(); // cannon
	//AActor* ownerByOwner = owner != nullptr ? owner->GetOwner() : nullptr; // turrel

	//APlayerController* PC = GetWorld()->GetFirstPlayerController();
	//if (PC)
	//{
	//	ATankPlayerController* TPC = Cast<ATankPlayerController>(PC);
	//	if (TPC)
	//	{
	//		MousePos = TPC->GetMousePositionCrude();

	//		//MousePos = TPC->GetMousePosition();

	//		//currentRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MousePos);
	//		//FRotator turretRotation = GetActorRotation();
	//		//currentRotation.Pitch = turretRotation.Pitch;
	//		//currentRotation.Roll = turretRotation.Roll;

	//		//FRotator NewRotation = FMath::Lerp(turretRotation, currentRotation, TurretRotationInterpolationKey);
	//		//TurretMesh->SetWorldRotation(NewRotation);

	//		//FVector turretPos = GetActorLocation();
	//		//DrawDebugLine(GetWorld(), turretPos, MousePos, FColor::Green, false, 2.0f, 0, 5);
	//	}

	//}

	
	//float x = FVector::DistXY(GetActorLocation(), MousePos);

	//float d = sqrt(pow(MousePos.X - GetActorLocation().X, 2) + pow(MousePos.Y - GetActorLocation().Y, 2) + pow(0 - 0, 2) * 1.0);

	//GetActorLocation
	
	//GetDistance()
	//float x = GetDistance()

	//UE_LOG(LogTemp, Warning, TEXT("ForwardV         value is: %s"), *forward.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("GetActorLocation value is: %s"), *GetActorLocation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("MousePos         value is: %s"), *MousePos.ToString());
	//MousePos.Z =  GetActorLocation().Z;
	//FVector temp = MousePos - GetActorLocation();
	
	//FVector::
	//UE_LOG(LogTemp, Warning, TEXT("Mouse-ActorLocatiation is: %s"), *temp.ToString());

	//float Dist = FVector::DistXY(MousePos , GetActorLocation() );

	//UE_LOG(LogTemp, Warning, TEXT("GetActorLocation value is: %s"), *GetActorLocation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("MousePos         value is: %s"), *MousePos.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("X: %f. "), Dist);
	//UE_LOG(LogTemp, Warning, TEXT("X: %f. "), d);


	MoveVector =  forward * DistanceToMouse * TrajectorySimulationSpeed; // TrajectorySimulationSpeed = 1.5

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
			ReturnPool();
			//Destroy();
		}
		else
		{
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TragectoryPointIndex]);
			SetActorRotation(newRotation);
		}
	}
}