// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"
#include "TankPawn.h"
#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);

	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
}

void AAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
								  bool bFromSweep, const FHitResult& SweepResult)
{

	// первый способ
	// 
	//ATankPawn* playerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	//if (OtherActor == playerPawn)
	//{
	//	playerPawn->SetupCannon(CannonClass);
	//	Destroy();
	//}


	// второй способ
	//
	ATankPawn* playerPawn = Cast<ATankPawn>(OtherActor);
	if (OtherActor)
	{
		//CannonClass.GetDefaultObject()->setupProjectile(ProjectileClass);

		//playerPawn->SetupCannon(CannonClass, ProjectileClass);
		playerPawn->SetupCannon(CannonClass, Type, Ammo);
		//playerPawn->AddAmmo(Ammo);
		Destroy();
		//Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Mesh->SetVisibility(false);
	}

}