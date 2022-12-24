// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class ACannon;

UCLASS()
class TANKOGEDDON_API ATankPawn : public ABasePawn
{
	GENERATED_BODY()

public:
	ATankPawn();

	virtual void Tick(float DeltaTime) override;

	void MoveBase(float Value);

	void RotationBase(float Value);

	void ChangeMainCannon();

	virtual void ReloadAmmo() override;

protected:
	virtual void BeginPlay() override;

	// camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;


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
