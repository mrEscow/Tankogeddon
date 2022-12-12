// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	class ATankPawn* TankPawn;

	virtual void SetupInputComponent() override;

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void RotationForward(float Value);
};
