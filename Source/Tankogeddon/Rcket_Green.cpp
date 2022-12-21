// Fill out your copyright notice in the Description page of Project Settings.


#include "Rcket_Green.h"

ARcket_Green::ARcket_Green()
{
	MoveSpeed = 2500.0f;
	Damage = 10.0f;
	Type = ERocketType::SmallRocket;
}

void ARcket_Green::CollisionWith(class AActor* OtherActor)
{

	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

		OtherActor->Destroy();
		//Destroy();
		ReturnPool();

		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "BOOM");
	}

}