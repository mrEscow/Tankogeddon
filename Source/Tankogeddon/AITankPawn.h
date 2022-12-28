// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "AITankPawn.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API AAITankPawn : public ABasePawn
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void MoveBase(float Value);

	void RotationBase(float Value);

	UFUNCTION()
	TArray<FVector> GetPatrollingPoints() { return PatrollingPoints; };

	UFUNCTION()
	float GetMovementAccurency() { return MovementAccurency; };

	UFUNCTION()
	FVector GetTurretForwardVector() { return TurretMesh->GetForwardVector(); }

	UFUNCTION()
	void RotateTurretTo(FVector TargetPosition);

	FVector GetEyesPosition(); 

protected:
	// move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Patrol points" , Meta = (MakeEditWidget = true))
	TArray<FVector> PatrollingPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Accurency")
	float MovementAccurency = 50;

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
	float moveBaseAxisValue = 0.0f;
	float rotationBaseAxisValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change Cannon Timer")
	float ChangeCannonTimer = 8;

private:
	void MoveAndRotationBase(float DeltaTime);
};
