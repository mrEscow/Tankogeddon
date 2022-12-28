// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DamageTaker.h"
#include "Scorable.h"
#include "BasePawn.generated.h"

class UStaticMeshComponent;

UCLASS()
class TANKOGEDDON_API ABasePawn : public APawn, public IDamageTaker, public IScorable
{
	GENERATED_BODY()

public:
	ABasePawn();

	void Fire();

	void SetupCannon(TSubclassOf<class ACannon> NewRocketCannonClass, ERocketType NewRocketType = ERocketType::NonType, int32 NewAmmo = 0);

	virtual void BeginPlay() override;

	virtual void ReloadAmmo();

	virtual void ChangeMainCannon();

	virtual void AddAmmo(int32 AmmoCount);

protected:
	// ”наследовано через IScorable
	virtual int32 GetScore() override;

	void ScoreTaked(int32 NewScore);

	// ”наследовано через IDamageTaker
	virtual void TakeDamage(FDamageData DamageData) override;


	UFUNCTION()
	void Die(AActor* DamageMaker);

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

	// score
	int32 MyScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScoreForMyDia")
	int32 ScoreForMyDia = 100; // скорость полЄта снар€да

};