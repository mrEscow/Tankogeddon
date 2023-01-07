// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"



UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHealthComponent::TakeDamage(FDamageData DamageData)
{
	float takedDamageValue = DamageData.DamageValue;

	CurrentHealth -= takedDamageValue;

	UE_LOG(LogTemp, Warning, TEXT("CurrentHealth: %f"), CurrentHealth);

	if (CurrentHealth <= 0)
	{
		if (OnDie.IsBound())
		{
			OnDie.Broadcast(DamageData.DamageMaker);
		}
	}
	else
	{
		if (OnDamaged.IsBound())
		{
			OnDamaged.Broadcast(takedDamageValue);
		}
	}

}

float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UHealthComponent::GetHealthState() const
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::AddHealth(float AddiditionalHealthValue)
{
	CurrentHealth += AddiditionalHealthValue;

	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}

}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	UE_LOG(LogTemp, Warning, TEXT("CurrentHealth: %f"), CurrentHealth);
}
