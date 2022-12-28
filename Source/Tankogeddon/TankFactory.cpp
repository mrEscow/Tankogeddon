#include "TankFactory.h"

#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "AITankPawn.h"


ATankFactory::ATankFactory()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComponent;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(sceneComponent);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->AttachToComponent(sceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(sceneComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Healthcomponent"));
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ATankFactory::DamageTaked);

}

void ATankFactory::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle spawnTimer;
	GetWorld()->GetTimerManager().SetTimer(spawnTimer, this, &ATankFactory::SpawnNewTank, SpawnTankRate, true, SpawnTankRate);

}

void ATankFactory::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankFactory::Die(AActor* DamageMaker)
{
	Destroy();
}

void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Factory %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
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
