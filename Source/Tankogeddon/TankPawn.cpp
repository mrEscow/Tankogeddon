// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPawn.h"

#include "TankPlayerController.h"

//#include "Projectile.h"
#include "Cannon.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"


ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxCollision;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);
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

void ATankPawn::RocketFire()
{
	if (RocketCannon)
	{
		RocketCannon->Fire();
	}
}

void ATankPawn::MashinGunFire()
{
	if (MachinGunCannon)
	{
		MachinGunCannon->Fire();
	}
}

void ATankPawn::LaserFire()
{
	if (LaserCannon)
	{
		LaserCannon->Fire();
	}
}

void ATankPawn::ChangeRocketType()
{

}

void  ATankPawn::ReloadAmmo()
{
	if (RocketCannon)
	{
		RocketCannon->ReloadAmmo();
	}
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> newRocketCannonClass, /*TSubclassOf< AProjectile> newProjectileClass,*/  int32 ammoCount)
{
	if (!newRocketCannonClass)
	{
		return;
	}


	if (RocketCannon)
	{
		RocketCannon->Destroy();
	}

	FActorSpawnParameters spawnParams;
	spawnParams.Instigator = this;
	spawnParams.Owner = this;

	RocketCannon = GetWorld()->SpawnActor<ACannon>(newRocketCannonClass, spawnParams);

	RocketCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	//if (ProjectileClass)
	//{
	//	RocketCannon->setupProjectile(ProjectileClass);
	//}
		
	if (ammoCount > 0)
	{
		AddAmmo(ammoCount);
	}
}

void ATankPawn::AddAmmo(int32 AmmoCount)
{
	if (RocketCannon)
	{
		RocketCannon->AddAmmo(AmmoCount);
		RocketCannon->ReloadAmmo();
	}
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	TankController = Cast<ATankPlayerController>(GetController());

	SetupCannon(CannonClass, 1);
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

	SetActorLocationAndRotation(NewPosition, NewRotation, false, 0, ETeleportType::None);
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
