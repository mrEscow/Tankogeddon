// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPawn.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Cannon.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

#include "HealthComponent.h"



ATurretPawn::ATurretPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxCollider;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(BoxCollider);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh, "ADD_Parts_Here_Socket");

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

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

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Healthcomponent"));
	HealthComponent->OnDie.AddUObject(this, &ATurretPawn::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ATurretPawn::DamageTaked);

}

void ATurretPawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATurretPawn::Die()
{
	Destroy();
}

void ATurretPawn::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
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

void ATurretPawn::Destroyed()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}

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

void ATurretPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}

}
