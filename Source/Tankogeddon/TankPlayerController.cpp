// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveTank", this, &ATankPlayerController::MoveTank);
	InputComponent->BindAxis("RotationTank", this, &ATankPlayerController::RotationTank);

	InputComponent->BindAction("Fire",EInputEvent::IE_Pressed ,this, &ATankPlayerController::Fire);
	InputComponent->BindAction("FireSpecial", EInputEvent::IE_Pressed, this, &ATankPlayerController::FireSpecial);

	InputComponent->BindAction("ReloadAmmo", EInputEvent::IE_Pressed, this, &ATankPlayerController::ReloadAmmo);
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
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::MoveTank(float Value)
{
	if (TankPawn)
	{
		TankPawn->MoveBase(Value);
	}
}

void ATankPlayerController::RotationTank(float Value)
{
	if (TankPawn)
	{
		TankPawn->RotationBase(Value);
	}
}

void ATankPlayerController::Fire()
{
	if (TankPawn)
	{
		TankPawn->Fire();
	}
}

void ATankPlayerController::FireSpecial()
{
	if (TankPawn)
	{
		TankPawn->FireSpecial();
	}
}

void  ATankPlayerController::ReloadAmmo()
{
	if (TankPawn)
	{
		TankPawn->ReloadAmmo();
	}
}
