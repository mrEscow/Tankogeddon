// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket_Red.h"
#include "DrawDebugHelpers.h"

ARocket_Red::ARocket_Red()
{
	MoveSpeed = 2000.0f;
	Damage = 30.0f;
	Type = ERocketType::BigRocket;
}

void ARocket_Red::CollisionWith(class AActor* OtherActor)
{

	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

		OtherActor->Destroy();
		//Destroy();
		ReturnPool();

		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Red, "BOOM");
	}

}
