// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectilePool.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AProjectilePool::AProjectilePool()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectilePool"));
	RootComponent = sceeneCpm;

	PoolPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawnpoint"));
	PoolPoint->SetupAttachment(sceeneCpm);

}

AProjectile* AProjectilePool::Get(TSubclassOf<class AProjectile> ProjectileClass, ERocketType RocketType)
{
	for (auto Projectile : Pool)
	{
		if (!Projectile->InActive()  && Projectile->GetType() == RocketType)
		{
			Projectile->SetPoolPoint(PoolPoint);
			return Projectile;
		}
	}

	// если не нашли свободный объект, то расширяем пул
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass);
	Projectile->SetPoolPoint(PoolPoint);
	Projectile->SetType(RocketType);

	Pool.Add(Projectile);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "DebugMessage:AProjectilePool::AProjectilePool() SPAWN NEW!!");

	return Projectile;

}

void AProjectilePool::Return(AProjectile* object)
{
	// в реальности не удаляем, а лишь помечаем, что объект свободен
	for (auto Record : Pool)
	{
		if (Record == object)
		{
			Record->ReturnPool();

			break;
		}
	}

}


void AProjectilePool::BeginPlay()
{
	Super::BeginPlay();

}
