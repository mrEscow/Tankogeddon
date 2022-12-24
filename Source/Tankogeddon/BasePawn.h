// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DamageTaker.h"
#include "BasePawn.generated.h"

class UStaticMeshComponent;

UCLASS()
class TANKOGEDDON_API ABasePawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:
	ABasePawn();

	void Fire();

	void SetupCannon(TSubclassOf<class ACannon> NewRocketCannonClass, ERocketType NewRocketType = ERocketType::NonType, int32 NewAmmo = 0);

protected:
	virtual void BeginPlay() override;

	virtual void ReloadAmmo();

	void AddAmmo(int32 AmmoCount);

	// ������������ ����� IDamageTaker
	virtual void TakeDamage(FDamageData DamageData) override;

	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaked(float DamageValue);

protected:
	// body
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	// health
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

	// cannon
	UPROPERTY()
	class ACannon* Cannon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	TSubclassOf<ACannon> CannonClassSecond;

	ERocketType CannonRocketTypeSecond = ERocketType::NonType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	class UArrowComponent* CannonSetupPoint;

};
