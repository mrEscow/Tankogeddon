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

	DECLARE_EVENT_OneParam(AProjectile, FOnKill, int32 Score)

public:
	FOnKill OnKill;

	void TakeScore(int32 NewScore);

public:
	ACannon();

	void Fire();
	void AddAmmo(int32 AmmoCount);
	void ReloadAmmo();
	void AutoShyting();

	int32 GetAllAmmo() { return CurrentCountAmmo + CountAmmo; }

	void SetRocketType(ERocketType NewRocketType);
	ERocketType GetRocketType();

	bool IsReadyToFire();

private:
	void FireProjectileShut();
	void FireTraceShut();
	void FireMashinGun();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent * Mesh; // модель пушки

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint; // точка, из которой будет производиться стрельба


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate = 5.0f; // частота стрельбы, сколько выстрелов в секунду делает наша пушка.

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRange = 2500.0f; // дистанция стрельбы

	// тип и класс снаряда
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ECannonType Type = ECannonType::FireRocket; //тип пушки

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ERocketType RocketType = ERocketType::NonType; //тип ракет


	// снаряд
	UPROPERTY()
	class AProjectile* Projectile; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	TSubclassOf<class AProjectile> ProjectileClass; 


	// Ammo
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params/Ammo")
	int32 MaxCurrentAmmo = 5; // количество снарядов

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params/Ammo")
	int32 CurrentCountAmmo = 0; // количество снарядов

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params/Ammo")
	int32 CountAmmo = 50; // общее количество снарядов

	// auto shuting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auto Shuting")
	int AutoShutCount = 3; // количество снарядов

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auto Shuting")
	float AutoShutTme = 0.02f; // время повторного выстрела

	// effects
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	class UParticleSystemComponent* ShootEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	class UAudioComponent* AudioEffectRocket;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	class UAudioComponent* AudioEffectLaser;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class UForceFeedbackEffect* ShootForceEffect;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> ShootShake;

private:
	FTimerHandle ReloadCannonTimerHandle; // структура, использующаяся для работы с таймерами перезарядки оружия.

	FTimerHandle ReloadAmmoTimerHandle; // структура, использующаяся для работы с таймером перезарядки патрон.

	FTimerHandle AutomaticShootingTimerHandle; // структура, использующаяся для работы с таймером для автоматической стрельбы


	bool ReadyToFire = false; // признак готовности пушки к стрельбе

	bool isReloadWeapon = false; // идет ли перезарядка

	bool isAutoShyting = false; // идет ли автоматическая стрельба

	virtual void BeginPlay() override;

	void Reload();

	class AGameSingleton* GameSingleton;

	void ShootEffects();
};
