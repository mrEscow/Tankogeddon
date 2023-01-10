// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"


#include "GameSingleton.h"
#include "Engine/World.h"
#include "DamageTaker.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraShakeBase.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawnpoint"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShootEffect"));
	ShootEffect->SetupAttachment(ProjectileSpawnPoint);
	ShootEffect->SetAutoActivate(false);

	AudioEffectRocket = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioEffectRocket"));
	AudioEffectRocket->SetupAttachment(sceeneCpm);
	AudioEffectRocket->SetAutoActivate(false);

	AudioEffectLaser = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioEffectLaser"));
	AudioEffectLaser->SetupAttachment(sceeneCpm);
	AudioEffectLaser->SetAutoActivate(false);


}


void ACannon::Fire()
{
	if (!IsReadyToFire())
	{
		return;
	}

	switch (Type)
	{
	case ECannonType::FireRocket:

		// AutomaticShooting
		if (!isAutoShyting)
		{
			if (CurrentCountAmmo == 0)
			{
				ReloadAmmo();
				return;
			}

			CurrentCountAmmo--;

			ShutCount = AutoShutCount;

			AutoShyting();
		}
		break;

	case ECannonType::FireLaser:

		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - trace");
		FireTraceShut();

		break;
	case ECannonType::FireMachineGun:
		FireMashinGun();
		break;
	default:
		return;
		break;
	}

	ReadyToFire = false;

	// управление таймером через глобальный объект TimerManager
	GetWorld()->GetTimerManager().SetTimer(ReloadCannonTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}

void ACannon::TakeScore(int32 NewScore)
{
	if (OnKill.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("CANNON: %s Score:  %d"), *GetName(), NewScore);

		OnKill.Broadcast(NewScore);
	}
}


void ACannon::AddAmmo(int32 AmmoCount)
{
	CountAmmo += AmmoCount;
}

void ACannon::AutoShyting()
{
	if (ShutCount > 0)
	{
		isAutoShyting = true;

		// если идет перезарядка пушки то останавливаем
		if (GetWorld()->GetTimerManager().IsTimerActive(ReloadCannonTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(ReloadCannonTimerHandle);
		}

		// выстрел
		const FString DebugMessage = FString::Printf(TEXT("AUTO FIRE:\nAutoShutCount:  %d\nAmmo:   %d   count!"), AutoShutCount, CurrentCountAmmo);
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Orange, DebugMessage);

		FireProjectileShut();

		ShutCount--;

		// повтор
		GetWorld()->GetTimerManager().SetTimer(AutomaticShootingTimerHandle, this, &ACannon::AutoShyting, AutoShutTme, true);
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

		//// возвращаем количество выстрелов
		//ShutCount = 3;

		isAutoShyting = false;
	}
}


void ACannon::SetRocketType(ERocketType NewRocketType)
{
	RocketType = NewRocketType;
}

ERocketType ACannon::GetRocketType()
{
	return RocketType;
}



void ACannon::FireProjectileShut()
{
	if (!ProjectilePool)
	{
		SetPool();
	}
	else
	{
		AProjectile* NewProjectile = ProjectilePool->Get(ProjectileClass, RocketType);

		if (NewProjectile)
		{
			ShootEffects();

			NewProjectile->OnKill.AddUObject(this, &ACannon::TakeScore);

			NewProjectile->Start(ProjectileSpawnPoint, FireRange);
		}
	}

}

void ACannon::FireTraceShut()
{
	FHitResult hitResult;

	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	traceParams.AddIgnoredActor(this);
	traceParams.bTraceComplex = true;
	traceParams.bReturnPhysicalMaterial = false;

	FVector start = ProjectileSpawnPoint->GetComponentLocation();
	FVector end = ProjectileSpawnPoint->GetForwardVector() * FireRange + start;

	if (AudioEffectLaser)
	{
		AudioEffectLaser->Play();
	}

	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
	{
		DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Red, false, 2.0f, 0, LaserBold);

		if (hitResult.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor %s. "),  *hitResult.GetActor()->GetName());

			//hitResult.GetActor()->Destroy();

			float Damage = 50.0f;

			AActor* owner = GetOwner();

			IDamageTaker* damageTakerActor = Cast<IDamageTaker>(hitResult.GetActor());

			if (damageTakerActor)
			{
				FDamageData damageData;
				damageData.DamageValue = Damage;
				damageData.Instigator = owner;
				damageData.DamageMaker = this;
				damageTakerActor->TakeDamage(damageData);
			}
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), start, end, FColor::Yellow, false, 2.0f, 0, LaserBold);
	}
}

void ACannon::FireMashinGun()
{

}

void ACannon::ReloadAmmo()
{
	if (CurrentCountAmmo == MaxCurrentAmmo /* || CurrentCountAmmo  CountAmmo */)
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
	else if (CountAmmo > 0 && CurrentCountAmmo < MaxCurrentAmmo)
	{
		// если идет перезарядка пушки то останавливаем
		if (GetWorld()->GetTimerManager().IsTimerActive(ReloadCannonTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(ReloadCannonTimerHandle);
		}

		CountAmmo--;
		CurrentCountAmmo++;

		const FString DebugMessage = FString::Printf(TEXT("Reloading Ammo:   %d   count!"), CurrentCountAmmo);
		GEngine->AddOnScreenDebugMessage(15, 2.0f, FColor::Yellow, DebugMessage);

		GetWorld()->GetTimerManager().SetTimer(ReloadAmmoTimerHandle, this, &ACannon::ReloadAmmo, 0.3f, false, true);

		isReloadWeapon = true;
	}
	else
	{
		isReloadWeapon = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "No ammo!");
	}
	
}


bool ACannon::IsReadyToFire()
{
	if (CurrentCountAmmo == 0)
	{
		if (CountAmmo > 0)
		{
			ReloadAmmo();
		}

		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Reload your weapon!!!  R!");
	}

	if (isReloadWeapon)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "Reload Ammo! Wait!");
	}

	if (!ReadyToFire)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "Reload Cannon! Wait!");
	}

	if (isAutoShyting)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "Auto Shuting! Wait!");
	}
		
	return ReadyToFire  && CurrentCountAmmo > 0 && !isReloadWeapon && !isAutoShyting;

}

void ACannon::Reload()
{
	ReadyToFire = true;

}

void ACannon::SetPool()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ProjectilePool = AGameSingleton::Get()->GetProjectilePool(World);
	}
}

void ACannon::ShootEffects()
{
	if (ShootEffect)
	{
		ShootEffect->ActivateSystem();
	}

	if (AudioEffectRocket)
	{
		AudioEffectRocket->Play();
	}

	if (GetOwner() && GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (ShootForceEffect)
		{
			FForceFeedbackParameters shootForceEffectParams;
			shootForceEffectParams.bLooping = false;
			shootForceEffectParams.Tag = "shootForceEffectParams";

			GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ShootForceEffect, shootForceEffectParams);
		}

		if (ShootShake)
		{
			GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(ShootShake);
		}

	}
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();

	SetPool();

	Reload();

}

