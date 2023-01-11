// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "AITankPawn.h"
#include "TankPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	Initialize();

}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!AITankPawn)
	{
		Initialize();
	}

	if (!AITankPawn)
	{
		return;
	}

	AITankPawn->MoveBase(1.0f);

	float rotationValue = GetRotationgValue();

	AITankPawn->RotationBase(rotationValue);

	Targeting();

}

float ATankAIController::GetRotationgValue()
{
	FVector currentPoint;

	//if (PatrollingPoints.Num() == 0)
	//{
	//	currentPoint = AITankPawn->GetActorLocation();
	//}
	//else
	//{
	currentPoint = PatrollingPoints[CurrentPatrolPointIndex];
	//}

	FVector pawnLocation = AITankPawn->GetActorLocation();

	if (FVector::Distance(currentPoint, pawnLocation) <= MovementAccurency)
	{
		CurrentPatrolPointIndex++;

		if (CurrentPatrolPointIndex >= PatrollingPoints.Num())
		{
			CurrentPatrolPointIndex = 0;
		}

	}

	FVector moveDirection = currentPoint - pawnLocation;
	moveDirection.Normalize();

	FVector forwardDirection = AITankPawn->GetActorForwardVector();

	FVector rightDirection = AITankPawn->GetActorRightVector();

	//DrawDebugLine(GetWorld(), pawnLocation, currentPoint, FColor::Green, false, 0.1f, 0, 5);

	float forwardAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(forwardDirection, moveDirection)));

	float rightAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(rightDirection, moveDirection)));

	float rotationValue = 0.0f;

	if (forwardAngle > 5.0f)
	{
		rotationValue = 1.0f;
	}

	if (rightAngle > 90.0f)
	{
		rotationValue = -rotationValue;
	}

	//UE_LOG(LogTemp, Warning, TEXT("AI Rotation forwardAngle: %f rightAngle: %f rotationValue:% f"), forwardAngle, rightAngle, rotationValue);

	return rotationValue;

}

void ATankAIController::Targeting()
{
	if (!PlayerPawn) // if he is dia or not in game
	{
		PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

		return;
	}

	if (CanFire())
	{
		Fire();
	}

}

void ATankAIController::RotateToPlayer()
{
	AITankPawn->RotateTurretTo(PlayerPawn->GetActorLocation());

}

bool ATankAIController::IsPlayerInRange()
{
	return FVector::Distance(AITankPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) <= TargetingRange;

}

bool ATankAIController::CanFire()
{
	if (!IsPlayerSeen() || !IsPlayerInRange())
	{
		return false;
	}

	RotateToPlayer();

	FVector targetingDir = AITankPawn->GetTurretForwardVector();

	FVector dirToPlayer = PlayerPawn->GetActorLocation() - AITankPawn->GetActorLocation();
	dirToPlayer.Normalize();

	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir, dirToPlayer)));

	return aimAngle <= Accurency;
}

void ATankAIController::Fire()
{
	AITankPawn->Fire();
}

bool ATankAIController::IsPlayerSeen()
{
	if (!PlayerPawn)
	{
		Initialize();
	}

	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = AITankPawn->GetEyesPosition();

	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	traceParams.bTraceComplex = true;
	traceParams.AddIgnoredActor(AITankPawn);
	traceParams.bReturnPhysicalMaterial = false;

	FHitResult hitResult;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos, ECollisionChannel::ECC_Visibility, traceParams))
	{
		if (hitResult.GetActor() == PlayerPawn)
		{
			//DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Green, false, 0.5f, 0, 10);

			return true;
		}
	}

	//DrawDebugLine(GetWorld(), eyesPos, playerPos, FColor::Cyan, false, 0.5f, 0, 10);

	return false;

}

void ATankAIController::Initialize()
{
	AITankPawn = Cast<AAITankPawn>(GetPawn());

	if (AITankPawn)
	{
		PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

		FVector pawnLocation = AITankPawn->GetActorLocation();

		MovementAccurency = AITankPawn->GetMovementAccurency();

		TArray<FVector> points = AITankPawn->GetPatrollingPoints();

		for (FVector point : points)
		{
			PatrollingPoints.Add(point);
		}

		CurrentPatrolPointIndex = 0;
	}
}