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

	void Start();

	// for pool
	bool InActive() { return IsActive; }
	void SetActive(bool Active) { IsActive = Active; }
	//void SetType(ERocketType NewType) { Type = NewType; }
	ERocketType GetType() { return Type; }
	void SetTimeLive(float Range) { TimeLive = Range / MoveSpeed; }
	void SetHomePoint(UArrowComponent* HomePoint) { ProjectileHomePoint = HomePoint; }
	
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
	float Damage = 1; // повреждения, которые будет наносить снаряд при попадании.

	FTimerHandle MovementTimerHandle;

private:
	//for pool
	FTimerHandle LiveTimerHandle;
	float TimeLive = 10;
	UArrowComponent* ProjectileHomePoint; // точка, где хранятся снаряды
protected:
	void ReturnPool();

protected:
	UFUNCTION() // обнаружение столкновения с другими объектами.
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor*
			OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool
			bFromSweep, const FHitResult& SweepResult);

	virtual void CollisionWith(class AActor* OtherActor);

	UFUNCTION()
	void Move();

private:
	bool IsActive = false;

	int32 Score = 0;

};
