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
		
		// AutomaticShooting
		if (!isAutoShyting)
		{
			Ammo--;

			AutoShyting();
		}

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
	GetWorld()->GetTimerManager().SetTimer(ReloadCannonTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}

void ACannon::AutoShyting()
{
	if (AutoShutCount > 0)
	{
		isAutoShyting = true;

		// если идет перезарядка пушки то останавливаем
		if (GetWorld()->GetTimerManager().IsTimerActive(ReloadCannonTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(ReloadCannonTimerHandle);
		}

		// выстрел
		const FString DebugMessage = FString::Printf(TEXT("AUTO FIRE:\nAutoShutCount:  %d\nAmmo:   %d   count!"), AutoShutCount, Ammo);
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Orange, DebugMessage);
		AutoShutCount--;

		// повтор
		GetWorld()->GetTimerManager().SetTimer(AutomaticShootingTimerHandle, this, &ACannon::AutoShyting, AutoShutTme, false, true);
	}
	else
	{
		// если серия автоматических выстрелов закончина то запускаем зарядку оружия
		if (!GetWorld()->GetTimerManager().IsTimerActive(ReloadCannonTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(ReloadCannonTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
		}

		// отключаем повтор
		if (GetWorld()->GetTimerManager().IsTimerActive(AutomaticShootingTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(AutomaticShootingTimerHandle);
		}

		// возвращаем количество выстрелов
		AutoShutCount = 3;

		isAutoShyting = false;
	}
}

void ACannon::FireSpecial()
{
	if (!IsReadyToFire())
	{
		return;
	}

	// swith ругается на пеедачу управления поэтому пришлось вынести 
	int32 Key = -1;
	float TimeToDisplay = 1.0f;
	FColor DisplayColor = FColor::Blue;
	const FString DebugMessage = FString::Printf(TEXT("Fire - projectile,   Ammo:   %d   count!"), Ammo);

	switch (Type)
	{
	case ECannonType::FireProjectile:

		Ammo--;

		GEngine->AddOnScreenDebugMessage(Key , TimeToDisplay, DisplayColor , DebugMessage);

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
	GetWorld()->GetTimerManager().SetTimer(ReloadCannonTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}

void ACannon::ReloadAmmo()
{
	if (Ammo == 10)
	{
		isReloadWeapon = false;

		// если перезарядка патрон закончина то запускаем зарядку оружия
		if (!GetWorld()->GetTimerManager().IsTimerActive(ReloadCannonTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(ReloadCannonTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
		}

		// если идет перезарядка патрон то останавливаем
		if (GetWorld()->GetTimerManager().IsTimerActive(ReloadAmmoTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(ReloadAmmoTimerHandle);
		}

		GEngine->AddOnScreenDebugMessage(15, 2.0f, FColor::Green, "Ammo is full! GO!!!");

		return;
	}
	else
	{
		isReloadWeapon = true;

		// если идет перезарядка пушки то останавливаем
		if (GetWorld()->GetTimerManager().IsTimerActive(ReloadCannonTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(ReloadCannonTimerHandle);
		}

		Ammo++;

		const FString DebugMessage = FString::Printf(TEXT("Reloading Ammo:   %d   count!"), Ammo);
		GEngine->AddOnScreenDebugMessage(15, 2.0f, FColor::Yellow, DebugMessage);

		GetWorld()->GetTimerManager().SetTimer(ReloadAmmoTimerHandle, this, &ACannon::ReloadAmmo, 0.3f, false, true);

	}

}

bool ACannon::IsReadyToFire()
{
	if (Ammo == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Reload your weapon!!!  R!");
	}

	if (isReloadWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "Reload Ammo! Wait!");
	}

	if (!ReadyToFire)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "Reload Cannon! Wait!");
	}

	if (isAutoShyting)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "Auto Shuting! Wait!");
	}
		
	return ReadyToFire  && Ammo > 0 && !isReloadWeapon && !isAutoShyting;
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

