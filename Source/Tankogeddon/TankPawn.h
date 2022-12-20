// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class ACannon;
class AProjectile;

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:
	ATankPawn();

	virtual void Tick(float DeltaTime) override;

	void MoveBase(float Value);
	void RotationBase(float Value);

	void RocketFire();
	void MashinGunFire();
	void LaserFire();

	void ChangeRocketType();
	void ReloadAmmo();

	void SetupCannon(TSubclassOf<ACannon> newRocketCannonClass, /*TSubclassOf< AProjectile> newProjectileClass,*/  int32 ammoCount = 0);
	void AddAmmo(int32 ammoCount);

protected:
	virtual void BeginPlay() override;

	// body
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	// camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;

	// guns
	UPROPERTY()
	ACannon* RocketCannon;

	UPROPERTY()
	ACannon* MachinGunCannon;

	UPROPERTY()
	ACannon* LaserCannon;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	class UArrowComponent* CannonSetupPoint;

	// props
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 100.0f;

	// interpolation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InterpolationKey")
	float BaseMoveInterpolationKey = 0.125f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InterpolationKey")
	float BaseRotationInterpolationKey = 0.125f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InterpolationKey")
	float TurretRotationInterpolationKey = 0.125f;

	// controller
	UPROPERTY()
	class ATankPlayerController* TankController;

	float moveBaseAxisValue = 0.0f;
	float rotationBaseAxisValue = 0.0f;

private:
	void MoveAndRotationBase(float DeltaTime);

	void RotationTurrel(float DeltaTime);
};
