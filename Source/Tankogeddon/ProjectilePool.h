// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameStruct.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectilePool.generated.h"



class AProjectile;
class UArrowComponent;


UCLASS()
class TANKOGEDDON_API AProjectilePool : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectilePool();

	AProjectile* Get(TSubclassOf<class AProjectile> ProjectileClass, ERocketType RocketType);

	void Return(AProjectile* object);

	//virtual ~AProjectilePool();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<class AProjectile> ProjectileClass0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<class AProjectile> ProjectileClass1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<class AProjectile> ProjectileClass2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint; // точка, где хранятся снаряды

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	int32 PoolSize = 10;
private:
	//struct PoolRecord
	//{
	//	AProjectile* instance;
	//	bool in_use = false;
	//};

	TArray<AProjectile*> Pool;

};
