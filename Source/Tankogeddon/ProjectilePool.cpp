// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectilePool.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AProjectilePool::AProjectilePool()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectilePool"));
	RootComponent = sceeneCpm;

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawnpoint"));
	ProjectileSpawnPoint->SetupAttachment(sceeneCpm);

}

AProjectile* AProjectilePool::Get(TSubclassOf<class AProjectile> ProjectileClass, ERocketType RocketType)
{
	for (auto Record: Pool)
	{
		if (!Record->InActive()  && Record->GetType() == RocketType)
		{
			Record->SetActive(true); // ��������� ������ � ������ ������������
			return Record;
		}
	}

	// ���� �� ����� ��������� ������, �� ��������� ���
	AProjectile* record;

	record = GetWorld()->SpawnActor<AProjectile>(ProjectileClass);
	//record = NewObject<AProjectile>(ProjectileClass);

	record->SetHomePoint(ProjectileSpawnPoint);
	Pool.Add(record);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "DebugMessage:AProjectilePool::AProjectilePool() SPAWN NEW!!");

	return record;

}

void AProjectilePool::Return(AProjectile* object)
{
	// � ���������� �� �������, � ���� ��������, ��� ������ ��������
	for (auto Record : Pool)
	{
		if (Record == object)
		{
			Record->SetActive(false);
			Record->SetActorLocation(ProjectileSpawnPoint->GetComponentLocation());
			Record->SetActorRotation(ProjectileSpawnPoint->GetComponentRotation());
			break;
		}
	}

}

//AProjectilePool::~AProjectilePool()
//{
//	// ������ ��� "��-����������" ������� �������
//	for (auto Record : Pool)
//	{
//		 //Pool[i].instance->Destroy();
//		Pool.Pop();
//	}
//
//}

void AProjectilePool::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < PoolSize; i++)
	{
		if (ProjectileClass0)
		{
			AProjectile* record;

			FVector pos = ProjectileSpawnPoint->GetComponentLocation();
			pos.Y += i * 100; // �������� ��� �����������

			record = GetWorld()->SpawnActor<AProjectile>(ProjectileClass0,
				pos,
				ProjectileSpawnPoint->GetComponentRotation());

			//UArrowComponent* point  = CreateDefaultSubobject<UArrowComponent>(TEXT("Point"));
			//point->SetAllPhysicsPosition(pos);
			//point->SetAllPhysicsRotation(ProjectileSpawnPoint->GetComponentRotation());
			record->SetHomePoint(ProjectileSpawnPoint);

			Pool.Add(record);

			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Create projectile!");
		}

		if (ProjectileClass1)
		{
			AProjectile* record;

			FVector pos = ProjectileSpawnPoint->GetComponentLocation();
			pos.X += 400;
			pos.Y += i * 100; // �������� ��� �����������

			record = GetWorld()->SpawnActor<AProjectile>(ProjectileClass1,
				pos,
				ProjectileSpawnPoint->GetComponentRotation());

			//UArrowComponent* point = CreateDefaultSubobject<UArrowComponent>(TEXT("Point"));
			//point->SetAllPhysicsPosition(pos);
			//point->SetAllPhysicsRotation(ProjectileSpawnPoint->GetComponentRotation());
			record->SetHomePoint(ProjectileSpawnPoint);

			Pool.Add(record);

			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Create projectile!");
		}

		if (ProjectileClass2)
		{
			AProjectile* record;

			FVector pos = ProjectileSpawnPoint->GetComponentLocation();
			pos.X += 800;
			pos.Y += i * 100; // �������� ��� �����������

			record = GetWorld()->SpawnActor<AProjectile>(ProjectileClass2,
				pos,
				ProjectileSpawnPoint->GetComponentRotation());

			//UArrowComponent* point = CreateArrey CreateDefaultSubobject<UArrowComponent>(TEXT("Point"));
			//point->SetAllPhysicsPosition(pos);
			//point->SetAllPhysicsRotation(ProjectileSpawnPoint->GetComponentRotation());
			record->SetHomePoint(ProjectileSpawnPoint);

			Pool.Add(record);

			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Create projectile!");
		}

	}

}
