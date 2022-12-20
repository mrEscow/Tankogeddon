// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	// ��������� ������ �������
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	// �������� �� ������� ������ ����������� � ������ ��������,
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);

}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::Move, MoveRate, true, MoveRate);

}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
									 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									 bool bFromSweep, const FHitResult& SweepResult )
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

	CollisionWith(OtherActor);
	
}

void AProjectile::CollisionWith(AActor* OtherActor)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());
		OtherActor->Destroy();
		Destroy();
	}
}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;

	SetActorLocation(nextPosition);

}
