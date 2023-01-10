// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameStruct.h"

#include "Components/ArrowComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANKOGEDDON_API AProjectile : public AActor
{
	GENERATED_BODY()

	DECLARE_EVENT_OneParam(AProjectile, FOnKill, int32 NewScore)

public:
	FOnKill OnKill;

	void TakeScore(int32 NewScore);

public:
	AProjectile();

	virtual void Start(UArrowComponent* SpawnPoint, float Range);

	ERocketType GetType() { return Type; }

	void SetType(ERocketType NewType) { Type = NewType; }

	bool InActive() { return IsActive; }

	void SetPoolPoint(UArrowComponent* NewPoolPoint) { PoolPoint = NewPoolPoint; }

	void ReturnPool();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh; // 3D-модель снаряда

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	ERocketType Type = ERocketType::NonType; //тип пушки

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 100.0f; // скорость полёта снаряда

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveRate = 0.005f; // — частота обновления позиции снаряда в секундах

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage = 1; // повреждения, которые будет наносить снаряд при попадании

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float PushForce = 1000; // будет воздействовать на объекты при столкновении

	FTimerHandle MovementTimerHandle;

protected:
	//for pool
	FTimerHandle LiveTimerHandle;

	float TimeLive = 10;

	UArrowComponent* PoolPoint; // точка, где хранятся снаряды

protected:
	UFUNCTION() // обнаружение столкновения с другими объектами.
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor*
			OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool
			bFromSweep, const FHitResult& SweepResult);

	virtual void CollisionWith(class AActor* OtherActor);

	UFUNCTION()
	virtual void Move();

protected:
	bool IsActive = false;

	int32 Score = 0;

protected:
	void SetTimeLive(float Range) { TimeLive = Range / MoveSpeed; }

	void SetActive(bool Active) { IsActive = Active; }

};
