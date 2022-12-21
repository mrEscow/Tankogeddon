// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveTank", this, &ATankPlayerController::MoveTank);
	InputComponent->BindAxis("RotationTank", this, &ATankPlayerController::RotationTank);

	InputComponent->BindAction("RocketFire",EInputEvent::IE_Pressed ,this, &ATankPlayerController::RocketFire);
	InputComponent->BindAction("MashinGunFire", EInputEvent::IE_Pressed, this, &ATankPlayerController::MashinGunFire);
	InputComponent->BindAction("LaserFire", EInputEvent::IE_Pressed, this, &ATankPlayerController::LaserFire);

	InputComponent->BindAction("ChangeMainCannon", EInputEvent::IE_Pressed, this, &ATankPlayerController::ChangeMainCannon);
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

// move
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

// fire
void ATankPlayerController::RocketFire()
{
	if (TankPawn)
	{
		TankPawn->RocketFire();
	}
}

void ATankPlayerController::MashinGunFire()
{
	if (TankPawn)
	{
		TankPawn->MashinGunFire();
	}
}

void ATankPlayerController::LaserFire()
{
	if (TankPawn)
	{
		TankPawn->LaserFire();
	}
}

// options
void  ATankPlayerController::ChangeMainCannon()
{
	if (TankPawn)
	{
		TankPawn->ChangeMainCannon();
	}
}

void  ATankPlayerController::ReloadAmmo()
{
	if (TankPawn)
	{
		TankPawn->ReloadAmmo();
	}
}
