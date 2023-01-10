#pragma once

#include "CoreMinimal.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "TankFactory.generated.h"

UCLASS()
class ATankFactory : public AActor, public IDamageTaker
{
	GENERATED_BODY()

public:
	ATankFactory();

	UFUNCTION()
	void TakeDamage(FDamageData DamageData);

protected:
	virtual void BeginPlay() override;

	void SpawnNewTank();

	UFUNCTION()
	void Die(AActor* DamageMaker);

	UFUNCTION()
	void DamageTaked(float DamageValue);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BuildingMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* TankSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TSubclassOf<class AAITankPawn> SpawnTankClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn tanks params")
	float SpawnTankRate = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TArray<class ATargetPoint*> TankWayPoints;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	class AMapLoader* LinkedMapLoader;

	FTimerHandle spawnTimer;

	const FString BuildingMeshPathGood = "/Script/Engine.StaticMesh'/Game/BLENDER/TankFactory/good/TankFactory_RELISE.TankFactory_RELISE'";

	const FString BuildingMeshPathDead = "/Script/Engine.StaticMesh'/Game/BLENDER/TankFactory/dead/TankFactoryDead_RELISE.TankFactoryDead_RELISE'";

private:
	void LoadSecondLevel();



};
