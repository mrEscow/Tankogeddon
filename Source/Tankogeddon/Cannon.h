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
	UStaticMeshComponent * Mesh; // ������ �����

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint; // �����, �� ������� ����� ������������� ��������


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate = 1; // ������� ��������, ������� ��������� � ������� ������ ���� �����.

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRange = 1000; // ��������� ��������

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireDamage = 1; // ���������� ��������� �����������.

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ECannonType Type = ECannonType::FireProjectile; //��� �����.

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int Ammo = 10; // ���������� ��������

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Auto Shuting")
	int AutoShutCount = 3; // ���������� ��������

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Auto Shuting")
	float AutoShutTme = 0.1f; // ���������� ��������


	FTimerHandle ReloadCannonTimerHandle; // ���������, �������������� ��� ������ � ��������� ����������� ������.

	FTimerHandle ReloadAmmoTimerHandle; // ���������, �������������� ��� ������ � �������� ����������� ������.

	FTimerHandle AutomaticShootingTimerHandle; // ���������, �������������� ��� ������ � �������� ��� �������������� ��������


	bool ReadyToFire = false; // ������� ���������� ����� � ��������

	bool isReloadWeapon = false; // ���� �� �����������

	bool isAutoShyting = false; // ���� �� �����������

	virtual void BeginPlay() override;

	void Reload();
};
