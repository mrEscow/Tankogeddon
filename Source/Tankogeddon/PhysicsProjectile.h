// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "PhysicsProjectile.generated.h"

class UPhysicsComponent;

UCLASS()
class TANKOGEDDON_API APhysicsProjectile : public AProjectile
{
	GENERATED_BODY()
public:
	APhysicsProjectile();

	virtual void Start(UArrowComponent* SpawnPoint, float Range) override;

protected:
	virtual void Move() override;

	protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UPhysicsComponent* PhysicsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveAccurency = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Trajectory")
	float TrajectorySimulationMaxTime = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Trajectory")
	float TrajectorySimulationTimeStep = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Trajectory")
	float TrajectorySimulationSpeed = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Trajectory")
	bool ShowTrajectory = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|params")
	FVector MoveVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|params")
	TArray<FVector> CurrentTrajectory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|params")
	int32 TragectoryPointIndex;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|params")
	//float DistanceToMouse = 0;

};
