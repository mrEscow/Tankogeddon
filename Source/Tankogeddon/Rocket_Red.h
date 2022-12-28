// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Rocket_Red.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ARocket_Red : public AProjectile
{
	GENERATED_BODY()

public:
	ARocket_Red();

protected:
	void CollisionWith(class AActor* OtherActor) override;
};
