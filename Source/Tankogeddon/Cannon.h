// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStruct.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"

class UArrowComponent;

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()

public:
	ACannon();

	void Fire();

	void FireSpecial();

	void ReloadAmmo();

	bool IsReadyToFire();

	void AutoShyting();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent * Mesh; // модель пушки

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint; // точка, из которой будет производиться стрельба


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate = 1; // частота стрельбы, сколько выстрелов в секунду делает наша пушка.

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRange = 1000; // дистанция стрельбы

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireDamage = 1; // количество наносимых повреждений.

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ECannonType Type = ECannonType::FireProjectile; //тип пушки.

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int Ammo = 10; // количество снарядов

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Auto Shuting")
	int AutoShutCount = 3; // количество снарядов

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Auto Shuting")
	float AutoShutTme = 0.1f; // количество снарядов


	FTimerHandle ReloadCannonTimerHandle; // структура, использующаяся для работы с таймерами перезарядки оружия.

	FTimerHandle ReloadAmmoTimerHandle; // структура, использующаяся для работы с таймером перезарядки патрон.

	FTimerHandle AutomaticShootingTimerHandle; // структура, использующаяся для работы с таймером для автоматической стрельбы


	bool ReadyToFire = false; // признак готовности пушки к стрельбе

	bool isReloadWeapon = false; // идет ли перезарядка

	bool isAutoShyting = false; // идет ли перезарядка

	virtual void BeginPlay() override;

	void Reload();
};
