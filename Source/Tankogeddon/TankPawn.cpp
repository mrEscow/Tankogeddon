// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxCollision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

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

	SetLocationAndRotation(DeltaTime);
}

void ATankPawn::MoveForward(float Value)
{
	targetForwardAxisValue = Value;
}

void ATankPawn::RotationForward(float Value)
{
	targetRotationAxisValue = Value;
}

void ATankPawn::SetLocationAndRotation(float DeltaTime)
{
	FVector currentLocation = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();
	FVector NewPosition = currentLocation + forwardVector * MoveSpeed * targetForwardAxisValue * DeltaTime;

	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += targetRotationAxisValue * RotationSpeed * DeltaTime;
	FRotator NewRotation = FRotator(Rotation);

	SetActorLocationAndRotation(NewPosition, NewRotation, false, 0, ETeleportType::None);
}


