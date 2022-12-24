// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Rocket_Green.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ARocket_Green : public AProjectile
{
	GENERATED_BODY()
	
public:
	ARocket_Green();

protected:
	void CollisionWith(class AActor* OtherActor) override;

};

