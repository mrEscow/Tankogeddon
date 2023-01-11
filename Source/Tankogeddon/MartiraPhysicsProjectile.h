// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsProjectile.h"
#include "MartiraPhysicsProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API AMartiraPhysicsProjectile : public APhysicsProjectile
{
	GENERATED_BODY()
public:
	virtual void Start(UArrowComponent* SpawnPoint, float Range) override;

protected:
	virtual void Move() override;
};
