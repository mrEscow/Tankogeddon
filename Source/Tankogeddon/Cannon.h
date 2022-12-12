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

	bool IsReadyToFire();

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

	FTimerHandle ReloadTimerHandle; // структура, использующаяся для работы с таймерами.

	bool ReadyToFire = false; // признак готовности пушки к стрельбе

	virtual void BeginPlay() override;

	void Reload();
};
