// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

#include "DamageTaker.h"


void AProjectile::TakeScore(int32 NewScore)
{
	if (OnKill.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("TAKER: %s Score:  %d"), *GetName(), NewScore);

		OnKill.Broadcast(NewScore);
	}
}

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	// Установка канала колизии
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	// Подписка на событие начала пересечения с другим объектом,
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);

}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::Move, MoveRate, true, MoveRate);

	GetWorld()->GetTimerManager().SetTimer(LiveTimerHandle, this, &AProjectile::ReturnPool, TimeLive, false);
}

void AProjectile::ReturnPool()
{
	GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
	IsActive = false;
	SetActorLocation(ProjectileHomePoint->GetComponentLocation());
	SetActorRotation(ProjectileHomePoint->GetComponentRotation());
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
									 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									 bool bFromSweep, const FHitResult& SweepResult )
{
	AActor* Projectile = Cast<AProjectile>(OtherActor);

	if (Projectile)
	{
		return;
	}
	
	AActor* owner = GetOwner();
	AActor* ownerByOwner = owner != nullptr ? owner->GetOwner() : nullptr;

	if (OtherActor != owner && OtherActor != ownerByOwner)
	{
		IDamageTaker* damageTakerActor = Cast<IDamageTaker>(OtherActor);

		if (damageTakerActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

			FDamageData damageData;
			damageData.DamageValue = Damage;

			damageData.Instigator = owner;
			damageData.DamageMaker = this;

			damageTakerActor->TakeDamage(damageData);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WTF!"));
			//OtherActor->Destroy();
		}



		ReturnPool();
	}

	//virtual metod for chailds
	//CollisionWith(OtherActor);
	
}

void AProjectile::CollisionWith(AActor* OtherActor)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());
		//OtherActor->Destroy();
		//Destroy();
		ReturnPool();
	}
}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;

	SetActorLocation(nextPosition);

}
