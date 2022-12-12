// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("RotationForward", this, &ATankPlayerController::RotationForward);

	InputComponent->BindAction("Fire",EInputEvent::IE_Pressed ,this, &ATankPlayerController::Fire);
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector MouseDirection;
	DeprojectMousePositionToWorld(MousePos, MouseDirection);
	FVector PawnPosition = TankPawn->GetActorLocation();
	MousePos.Z = PawnPosition.Z;
	FVector dir = MousePos - PawnPosition;
	dir.Normalize();
	MousePos = PawnPosition + dir * 1000;

	DrawDebugLine(GetWorld(), PawnPosition, MousePos, FColor::Green, false, 0.1f, 0, 5);
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::MoveForward(float Value)
{
	if (TankPawn)
	{
		TankPawn->MoveForward(Value);
	}
}

void ATankPlayerController::RotationForward(float Value)
{
	if (TankPawn)
	{
		TankPawn->RotationForward(Value);
	}
}

void ATankPlayerController::Fire()
{
	if (TankPawn)
	{
		TankPawn->Fire();
	}
}
