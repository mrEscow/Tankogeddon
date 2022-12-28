// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "GameStruct.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8 {
	FireRocket = 0 UMETA(DisplayName = "Use rocket"),
	FireLaser = 1 UMETA(DisplayName = "Use laser"),
	FireMachineGun = 2 UMETA(DisplayName = "Use MachineGun")
};

UENUM(BlueprintType)
enum class ERocketType : uint8 {
	BigRocket = 0 UMETA(DisplayName = "Use BigRocket"),
	MidlRocket = 1 UMETA(DisplayName = "Use MidlRocket"),
	SmallRocket = 2 UMETA(DisplayName = "Use SmallRocket"),
	NonType = 3 UMETA(DisplayName = "Use NonType")
};


USTRUCT()
struct FDamageData
{
	GENERATED_BODY()

	UPROPERTY()
	float DamageValue;

	UPROPERTY()
	int32 Score;

	UPROPERTY()
	AActor* Instigator;

	UPROPERTY()
	AActor* DamageMaker;
};


UCLASS()
class TANKOGEDDON_API UGameStruct : public UObject
{
	GENERATED_BODY()
	
};
