// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawnpoint"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ACannon::Fire()
{
	if (!IsReadyToFire())
	{
		return;
	}
	
	switch (Type)
	{
	case ECannonType::FireProjectile:
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - projectile");
		break;

	case ECannonType::FireTrace:
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - trace");
		break;

	default:
		return;
		break;
	}

	ReadyToFire = false;

	// управление таймером через глобальный объект TimerManager
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}

bool ACannon::IsReadyToFire()
{
	return ReadyToFire /* && Ammo > 0*/;
}

void ACannon::Reload()
{
	ReadyToFire = true;
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();

	Reload();
}

