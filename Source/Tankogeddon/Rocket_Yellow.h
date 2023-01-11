// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Rocket_Yellow.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ARocket_Yellow : public AProjectile
{
	GENERATED_BODY()

public:
	ARocket_Yellow();

protected:
	void CollisionWith(class AActor* OtherActor) override;
};
