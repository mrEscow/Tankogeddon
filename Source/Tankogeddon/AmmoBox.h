// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameStruct.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoBox.generated.h"

UCLASS()
class TANKOGEDDON_API AAmmoBox : public AActor
{
	GENERATED_BODY()

public:
	AAmmoBox();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	TSubclassOf<class ACannon> CannonClass; // тип пушки

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 Ammo = 0; // количество снарядов

protected:
	UFUNCTION()
	void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
							bool bFromSweep, const FHitResult& SweepResult);

};
