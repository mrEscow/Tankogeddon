// Fill out your copyright notice in the Description page of Project Settings.


#include "AITankPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"


void AAITankPawn::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle ChangeCannonTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(ChangeCannonTimerHandle, this, &ABasePawn::ChangeMainCannon, ChangeCannonTimer, true, 0);
}

void AAITankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveAndRotationBase(DeltaTime);

}

void AAITankPawn::MoveBase(float Value)
{
	moveBaseAxisValue = Value;
}

void AAITankPawn::RotationBase(float Value)
{
	rotationBaseAxisValue = Value;
}

void AAITankPawn::MoveAndRotationBase(float DeltaTime)
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

void AAITankPawn::RotateTurretTo(FVector TargetPosition)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);

	FRotator currRotation = TurretMesh->GetComponentRotation();

	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;

	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TurretRotationInterpolationKey));
}

FVector AAITankPawn::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}
