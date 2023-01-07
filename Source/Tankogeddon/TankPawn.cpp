// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPawn.h"

#include "TankPlayerController.h"
#include "Cannon.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "HealthComponent.h"


ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveAndRotationBase(DeltaTime);

	RotationTurrel(DeltaTime);

}

void ATankPawn::MoveBase(float Value)
{
	moveBaseAxisValue = Value;
}

void ATankPawn::RotationBase(float Value)
{
	rotationBaseAxisValue = Value;
}

float ATankPawn::GetCurrentHealth()
{
	return HealthComponent->GetCurrentHealth();
}

float ATankPawn::GetMaxHealth()
{
	return HealthComponent->GetMaxHealth();
}

float ATankPawn::GetHealthState()
{
	return HealthComponent->GetHealthState();
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	TankController = Cast<ATankPlayerController>(GetController());

}

void ATankPawn::MoveAndRotationBase(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector ChangePosition = CurrentLocation + ForwardVector * MoveSpeed * moveBaseAxisValue * DeltaTime;
	FVector NewPosition = FMath::Lerp(ChangePosition, CurrentLocation, BaseMoveInterpolationKey);

	FRotator CurrentRotation = GetActorRotation();
	FRotator ChangeRotation = CurrentRotation;
	ChangeRotation.Yaw += rotationBaseAxisValue * RotationSpeed * DeltaTime;
	FRotator NewRotation = FMath::Lerp(ChangeRotation, CurrentRotation, BaseRotationInterpolationKey);

	SetActorLocationAndRotation(NewPosition, NewRotation, true, 0, ETeleportType::None);

}

void ATankPawn::RotationTurrel(float DeltaTime)
{
	if (TankController)
	{
		FVector mousePos = TankController->GetMousePosition();
		FRotator currentRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mousePos);
		FRotator turretRotation = TurretMesh->GetComponentRotation();
		currentRotation.Pitch = turretRotation.Pitch;
		currentRotation.Roll = turretRotation.Roll;

		FRotator NewRotation = FMath::Lerp(turretRotation, currentRotation, TurretRotationInterpolationKey);
		TurretMesh->SetWorldRotation(NewRotation);

		FVector turretPos = TurretMesh->GetComponentLocation();
		//DrawDebugLine(GetWorld(), turretPos, mousePos, FColor::Green, false, 0.1f, 0, 5);
	}

}


