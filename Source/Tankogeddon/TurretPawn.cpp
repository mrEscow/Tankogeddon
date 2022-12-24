// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPawn.h"

#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Cannon.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"



ATurretPawn::ATurretPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	TurretMesh->SetupAttachment(BaseMesh, "ADD_Parts_Here_Socket");

	UStaticMesh * turretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if (turretMeshTemp)
	{
		TurretMesh->SetStaticMesh(turretMeshTemp);
	}

	UStaticMesh* baseMeshTemp = LoadObject<UStaticMesh>(this, *BaseMeshPath);
	if (baseMeshTemp)
	{
		BaseMesh->SetStaticMesh(baseMeshTemp);
	}

}

void ATurretPawn::ReloadAmmo()
{
	if (Cannon)
	{
		Cannon->ReloadAmmo();
	}
}

void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters params;

	params.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);

	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FTimerHandle _targetingTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(_targetingTimerHandle, this, &ATurretPawn::Targeting, TargetingRate, true, TargetingRate);
}

void ATurretPawn::Targeting()
{
	if (!PlayerPawn)
	{
		return;
	}

	if (IsPlayerInRange())
	{
		RotateToPlayer();

		if (CanFire())
		{
			Fire();
		}
	}
}

void ATurretPawn::RotateToPlayer()
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());

	FRotator currRotation = TurretMesh->GetComponentRotation();

	targetRotation.Pitch = currRotation.Pitch;

	targetRotation.Roll = currRotation.Roll;

	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TargetingSpeed));
}

bool ATurretPawn::IsPlayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}

bool ATurretPawn::CanFire()
{
	FVector targetingDir = TurretMesh->GetForwardVector();

	FVector dirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();

	dirToPlayer.Normalize();

	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir, dirToPlayer))); // math

	return aimAngle <= Accurency;
}

