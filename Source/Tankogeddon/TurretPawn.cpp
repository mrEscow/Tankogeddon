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

	FTimerHandle targetingTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(targetingTimerHandle, this, &ATurretPawn::Targeting, TargetingRate, true, TargetingRate);

	FTimerHandle ChangeCannonTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(ChangeCannonTimerHandle, this, &ABasePawn::ChangeMainCannon, ChangeCannonTimer, true, 0);
}

void ATurretPawn::Targeting()
{
	if (!PlayerPawn)
	{
		PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

		return;
	}

	if (CanFire())
	{
		Fire();
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
	if (!IsPlayerSeen() || !IsPlayerInRange())
	{
		return false;
	}

	RotateToPlayer();

	FVector targetingDir = TurretMesh->GetForwardVector();

	FVector dirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	dirToPlayer.Normalize();

	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir, dirToPlayer))); // math

	return aimAngle <= Accurency;
}

bool ATurretPawn::IsPlayerSeen()
{
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = CannonSetupPoint->GetComponentLocation();

	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	traceParams.bTraceComplex = true;
	traceParams.AddIgnoredActor(this);
	traceParams.bReturnPhysicalMaterial = false;

	FHitResult hitResult;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos, ECollisionChannel::ECC_Visibility, traceParams))
	{
		if (hitResult.GetActor() == PlayerPawn)
		{
			DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Green, false, 0.5f, 0, 10);

			return true;
		}
	}

	DrawDebugLine(GetWorld(), eyesPos, playerPos, FColor::Cyan, false, 0.5f, 0, 10);

	return false;
}

