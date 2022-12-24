// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"

#include "Components/BoxComponent.h"
#include "HealthComponent.h"
#include "Cannon.h"
#include "Components/ArrowComponent.h"


ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxCollision;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Healthcomponent"));
	HealthComponent->OnDie.AddUObject(this, &ABasePawn::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ABasePawn::DamageTaked);
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();

	SetupCannon(CannonClass);
}

void ABasePawn::ReloadAmmo()
{
	if (Cannon)
	{
		Cannon->ReloadAmmo();
	}
}

void ABasePawn::SetupCannon(TSubclassOf<ACannon> NewRocketCannonClass, ERocketType NewRocketType, int32 NewAmmo)
{
	if (!NewRocketCannonClass)
	{
		return;
	}

	if (CannonClass)
	{
		CannonClassSecond = CannonClass;
		if (Cannon)
		{
			CannonRocketTypeSecond = Cannon->GetRocketType();
		}

	}

	CannonClass = NewRocketCannonClass;

	if (Cannon)
	{
		Cannon->Destroy();
	}

	FActorSpawnParameters spawnParams;
	spawnParams.Instigator = this;
	spawnParams.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, spawnParams);

	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	Cannon->SetRocketType(NewRocketType);

	if (NewAmmo > 0)
	{
		AddAmmo(NewAmmo);
	}
}

void ABasePawn::AddAmmo(int32 AmmoCount)
{
	if (Cannon)
	{
		Cannon->AddAmmo(AmmoCount);
		Cannon->ReloadAmmo();
	}
}

void ABasePawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void ABasePawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
	UE_LOG(LogTemp, Warning, TEXT("ATankPawn::TakeDamage(FDamageData DamageData)"));
}

void ABasePawn::Die()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
	Destroy();
}

void ABasePawn::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Tank %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}