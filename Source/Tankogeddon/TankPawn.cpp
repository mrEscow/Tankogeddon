// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"

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

	RotationTurrel(DeltaTime);
}

void ATankPawn::MoveForward(float Value)
{
	targetForwardAxisValue = Value;
}

void ATankPawn::RotationForward(float Value)
{
	targetRotationAxisValue = Value;
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	TankController = Cast<ATankPlayerController>(GetController());
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

void ATankPawn::RotationTurrel(float DeltaTime)
{
	if (TankController)
	{
		FVector mousePos = TankController->GetMousePosition();
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mousePos);
		FRotator turretRotation = TurretMesh->GetComponentRotation();
		targetRotation.Pitch = turretRotation.Pitch;
		targetRotation.Roll = turretRotation.Roll;
		TurretMesh->SetWorldRotation(FMath::Lerp(turretRotation, targetRotation, TurretRotationInterpolationKey));
	}
}
