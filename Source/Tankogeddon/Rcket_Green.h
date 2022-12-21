// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Rcket_Green.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ARcket_Green : public AProjectile
{
	GENERATED_BODY()
	
public:
	ARcket_Green();

protected:
	void CollisionWith(class AActor* OtherActor) override;

};

