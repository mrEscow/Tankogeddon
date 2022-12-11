#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:
	ATankPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
