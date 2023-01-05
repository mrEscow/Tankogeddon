#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "MapLoader.generated.h"

class UPointLightComponent;

UCLASS()
class AMapLoader : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* GatesMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* TriggerCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|Point")
	UPointLightComponent* ActivatedLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|Point")
	UPointLightComponent* DeactivatedLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
	FName LoadLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
	bool IsActivated = false;

public:
	AMapLoader();

	void SetIsActivated(bool NewIsActivated);

protected:

	virtual void BeginPlay() override;

	void SetActiveLights();

	UFUNCTION()
	void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);
};

