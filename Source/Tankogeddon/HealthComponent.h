// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStruct.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	// events
	DECLARE_EVENT_OneParam(UHealthComponent, FOnDie, AActor* DamageMaker)

	DECLARE_EVENT_OneParam(UHealthComponent, FOnHealthChanged, float)

public:
	FOnDie OnDie;

	FOnHealthChanged OnDamaged;

public:
	UHealthComponent();

	void TakeDamage(FDamageData DamageData);

	float GetCurrentHealth() const;

	float GetMaxHealth() const;

	float GetHealthState() const;

	void AddHealth(float AddiditionalHealthValue);


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health values")
	float MaxHealth = 10;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health values")
	float CurrentHealth;

};