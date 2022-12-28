// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

#include "DamageTaker.h"
#include "Scorable.h"
#include "BasePawn.h"


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
	OnKill.Clear();

	Score = 0;

	GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);

	SetActorLocation(ProjectileHomePoint->GetComponentLocation());
	SetActorRotation(ProjectileHomePoint->GetComponentRotation());

	IsActive = false;
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
	
	AActor* owner = GetOwner(); // cannon
	AActor* ownerByOwner = owner != nullptr ? owner->GetOwner() : nullptr; // turrel
	AActor* ownerByOwnerByOwner = ownerByOwner != nullptr ? ownerByOwner->GetOwner() : nullptr; // root

	if (OtherActor != owner && OtherActor != ownerByOwner && OtherActor != ownerByOwnerByOwner) // if(not me)
	{
		IDamageTaker* damageTakerActor = Cast<IDamageTaker>(OtherActor);

		IScorable* scorobleActor = Cast<IScorable>(OtherActor);

		if (damageTakerActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

			if (scorobleActor)
			{
				Score = scorobleActor->GetScore();
			}

			FDamageData damageData;
			damageData.DamageValue = Damage;
			damageData.Score = Score;
			damageData.Instigator = owner;
			damageData.DamageMaker = this;

			damageTakerActor->TakeDamage(damageData);
		}
		else if (!damageTakerActor && scorobleActor)
		{
			Score = scorobleActor->GetScore();
			TakeScore(Score);
			OtherActor->Destroy();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("STOP!"));
			Score = 0;
			//OtherActor->Destroy();
		}

		ReturnPool();
	}

	
}

void AProjectile::CollisionWith(AActor* OtherActor)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

		ReturnPool();
	}
}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;

	SetActorLocation(nextPosition);

}
