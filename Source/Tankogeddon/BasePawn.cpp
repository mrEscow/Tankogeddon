// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"

#include "Components/BoxComponent.h"
#include "HealthComponent.h"
#include "Cannon.h"
#include "Components/ArrowComponent.h"

#include "Projectile.h"


ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	//SetActorEnableCollision(true);
	bLockLocation = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxCollision;

	//BoxCollision->SetCollisionProfileName(FName("OverlapAll"));
	//BoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	//BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//BoxCollision->SetGenerateOverlapEvents(true);

	BoxCollision->SetCollisionProfileName(FName("BlockAll"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(BoxCollision);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);
	TurretMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

	//if (CannonClass)
	//{
	//	CannonClassSecond = CannonClass;
	//	if (Cannon)
	//	{
	//		CannonRocketTypeSecond = Cannon->GetRocketType();
	//	}

	//}

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

	Cannon->OnKill.AddUObject(this, &ABasePawn::ScoreTaked);

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

void ABasePawn::ChangeMainCannon()
{
	UE_LOG(LogTemp, Warning, TEXT("ABasePawn::ChangeMainCannon!"));
	if (CannonClassSecond)
	{
		TSubclassOf<ACannon> CannonClassTemp;

		// bable
		CannonClassTemp = CannonClass;
		CannonClass = CannonClassSecond;
		CannonClassSecond = CannonClassTemp;

		SetupCannon(CannonClass, CannonRocketTypeSecond, Cannon->GetAllAmmo());
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
}

void ABasePawn::Die(AActor* DamageMaker)
{
	AProjectile* MyKiller = Cast<AProjectile>(DamageMaker);

	if (MyKiller)
	{
		UE_LOG(LogTemp, Warning, TEXT("Killer %s  Kill %s and take Score:  %d"), *MyKiller->GetName(), *GetName(), ScoreForMyDia);

		MyKiller->TakeScore(ScoreForMyDia);
	}

	if (Cannon)
	{
		Cannon->Destroy();
	}

	Destroy();
}

void ABasePawn::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Pawn %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

int32 ABasePawn::GetScore()
{
	return ScoreForMyDia;
}

void ABasePawn::ScoreTaked(int32 NewScore)
{
	MyScore += NewScore;

	UE_LOG(LogTemp, Warning, TEXT("Pawn %s Score:  %d"), *GetName(), MyScore);
}
