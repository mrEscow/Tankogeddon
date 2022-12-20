// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket_Yellow.h"

ARocket_Yellow::ARocket_Yellow()
{
	MoveSpeed = 2500.0f;
	Damage = 20.0f;

}

void ARocket_Yellow::CollisionWith(class AActor* OtherActor)
{

	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

		OtherActor->Destroy();
		Destroy();

		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Yellow, "BOOM");
	}

}
