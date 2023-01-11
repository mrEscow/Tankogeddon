// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

#include "DamageTaker.h"
#include "Scorable.h"
#include "BasePawn.h"

#include "Particles/ParticleSystemComponent.h"


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
	RootComponent->SetMobility(EComponentMobility::Movable);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Traileffect"));
	TrailEffect->SetMobility(EComponentMobility::Movable);
	TrailEffect->SetupAttachment(RootComponent);

	// отключаем видимость
	TrailEffect->SetVisibility(false);

	// Установка канала колизии
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	// Подписка на событие начала пересечения с другим объектом,
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);

	// отключаем колизию
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// отключаем видимость
	Mesh->SetVisibility(false);
}

void AProjectile::Start(UArrowComponent* SpawnPoint, float Range)
{
	// Активируемся
	SetActive(true);

	// новые координаты 
	SetActorLocation(SpawnPoint->GetComponentLocation());
	SetActorRotation(SpawnPoint->GetComponentRotation());

	// включаем видимость
	TrailEffect->SetVisibility(true);

	// включаем видимость
	Mesh->SetVisibility(true);

	// включаем колизию
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// вычисляем время жизни
	SetTimeLive(Range);

	// запускаем таймер жизни
	GetWorld()->GetTimerManager().SetTimer(LiveTimerHandle, this, &AProjectile::TimeLiveOut, TimeLive, false);

	// поехали!
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::Move, MoveRate, true, MoveRate);

}



void AProjectile::ReturnPool()
{
	OnKill.Clear();
	Score = 0;

	// стоп движение
	GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);

	// если время жизни продолжается, останавливаем
	if (GetWorld()->GetTimerManager().IsTimerActive(LiveTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(LiveTimerHandle);
	}

	// отключаем видимость
	TrailEffect->SetVisibility(false);

	// отключаем колизию
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// отключаем видимость
	Mesh->SetVisibility(false);

	// возвращаемся в пул
	if (PoolPoint)
	{
		SetActorLocation(PoolPoint->GetComponentLocation());
		SetActorRotation(PoolPoint->GetComponentRotation());
	}


	// диактивация
	SetActive(false);
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
									 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									 bool bFromSweep, const FHitResult& SweepResult )
{
	AActor* owner = GetOwner(); // cannon
	AActor* ownerByOwner = owner != nullptr ? owner->GetOwner() : nullptr; // turrel
	AActor* ownerByOwnerByOwner = ownerByOwner != nullptr ? ownerByOwner->GetOwner() : nullptr; // root

	if (OtherActor != owner && OtherActor != ownerByOwner && OtherActor != ownerByOwnerByOwner) // if(not me)
	{
		if (IsExplode)
		{
			Explode();
		}
		else
		{
			BumpInto(OtherActor);
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

void AProjectile::Explode()
{
	FVector startPos = GetActorLocation();
	FVector endPos = startPos + FVector(0.1f);

	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
	params.AddIgnoredActor(this);
	params.bTraceComplex = true;
	params.TraceTag = "Explode Trace";

	TArray<FHitResult> AttackHit;

	FQuat Rotation = FQuat::Identity;

	bool sweepResult = GetWorld()->SweepMultiByChannel
	(
		AttackHit,
		startPos,
		endPos,
		Rotation,
		ECollisionChannel::ECC_Visibility,
		Shape,
		params
	);

	GetWorld()->DebugDrawTraceTag = "Explode Trace";

	if (sweepResult)
	{
		for (FHitResult hitResult : AttackHit)
		{
			AActor* OtherActor = hitResult.GetActor();

			if (!OtherActor)
			{
				continue;
			}

			AActor* Projectile = Cast<AProjectile>(OtherActor);

			if (Projectile)
			{
				continue;
			}

			BumpInto(OtherActor);

		}
	}

}

void AProjectile::BumpInto(AActor* OtherActor)
{
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
			//OtherActor->Destroy();
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("STOP!"));
			Score = 0;

			UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());

			if (mesh)
			{
				if (mesh->IsSimulatingPhysics())
				{
					FVector forceVector = OtherActor->GetActorLocation() - GetActorLocation();
					forceVector.Normalize();

					EName KostochkaNmae = NAME_None;

					bool isIgnorMassa = true;

					// ● если хотим один раз толкнуть элемент, лучше использовать AddImpulse().
					mesh->AddImpulse(forceVector * PushForce, KostochkaNmae, isIgnorMassa);
					// ● если нужно постоянное воздействие на объект(например, для эффекта ветра или гравитации), более подходящим будет AddForce().
					//mesh->AddForce(forceVector * PushForce, NAME_None, true);
				}
			}
		}
	}

}

void AProjectile::TimeLiveOut()
{
	if (IsExplode)
	{
		Explode();
	}

	ReturnPool();

}
