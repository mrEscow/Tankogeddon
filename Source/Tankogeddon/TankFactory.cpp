#include "TankFactory.h"

#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "AITankPawn.h"

#include "MapLoader.h"


ATankFactory::ATankFactory()
{
	//PrimaryActorTick.bCanEverTick = false;

	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComponent;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(sceneComponent);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->AttachToComponent(sceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(sceneComponent);

	UStaticMesh* BuildingMeshTemp = LoadObject<UStaticMesh>(this, *BuildingMeshPathGood);
	if (BuildingMeshTemp)
	{
		BuildingMesh->SetStaticMesh(BuildingMeshTemp);
	}

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ATankFactory::DamageTaked);

}

void ATankFactory::BeginPlay()
{
	Super::BeginPlay();

	if (LinkedMapLoader)
	{
		LinkedMapLoader->SetIsActivated(false);
	}

	GetWorld()->GetTimerManager().SetTimer(spawnTimer, this, &ATankFactory::SpawnNewTank, SpawnTankRate, true, SpawnTankRate);

}

void ATankFactory::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankFactory::Die(AActor* DamageMaker)
{
	//Destroy();

	GetWorld()->GetTimerManager().ClearTimer(spawnTimer);

	UStaticMesh* BuildingMeshTemp = LoadObject<UStaticMesh>(this, *BuildingMeshPathDead);
	if (BuildingMeshTemp)
	{
		BuildingMesh->SetStaticMesh(BuildingMeshTemp);
	}

	//HitCollider->Deactivate();

	FTimerHandle loadTimer;

	GetWorld()->GetTimerManager().SetTimer(spawnTimer, this, &ATankFactory::LoadSecondLevel, 1, false, 0);
}

void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Factory %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetCurrentHealth());
}

void ATankFactory::LoadSecondLevel()
{
	//UGameplayStatics::OpenLevel(GetWorld(), "SecondLevel");
	//LinkedMapLoader

	if (LinkedMapLoader)
	{
		LinkedMapLoader->SetIsActivated(true);
		UE_LOG(LogTemp, Warning, TEXT("Factory DEAD MapLoader is Activ !!!"));
	}

}

void ATankFactory::SpawnNewTank()
{
	FTransform spawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1));

	AAITankPawn* NewTank = GetWorld()->SpawnActorDeferred<AAITankPawn>(
		SpawnTankClass, 
		spawnTransform, 
		this, 
		nullptr, 
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

	NewTank->SetPatrollingPoints(TankWayPoints);

	UGameplayStatics::FinishSpawningActor(NewTank, spawnTransform);

}
