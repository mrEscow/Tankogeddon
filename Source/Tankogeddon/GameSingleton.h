// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
//#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"
#include "GameSingleton.generated.h"

class AProjectilePool;

UCLASS(Blueprintable, BlueprintType)
class TANKOGEDDON_API AGameSingleton : public AActor
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static AGameSingleton* Get();

	AProjectilePool* GetProjectilePool(UWorld* World);

protected:
	UPROPERTY()
	AProjectilePool* ProjectilePool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectilePool")
	TSubclassOf<AProjectilePool> ProjectilePoolClass;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectilePool")
	class UArrowComponent* ProjectileSpawnPoint; 
private:
	AGameSingleton();
};
