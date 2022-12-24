// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSingleton.h"

#include "ProjectilePool.h"
#include "Components/ArrowComponent.h"

#include "Engine/World.h"

AGameSingleton* AGameSingleton::Get()
{
    if (GEngine)
    {
        AGameSingleton* Instance = Cast<AGameSingleton>(GEngine->GameSingleton);
        return Instance;
    }
    return nullptr;
}

AGameSingleton::AGameSingleton()
{

}

AProjectilePool* AGameSingleton::GetProjectilePool(UWorld* World)
{
    if (!ProjectilePool)
    {

        if (World)
        {
            ProjectilePool = World->SpawnActor<AProjectilePool>(
                ProjectilePoolClass//, 
                //ProjectileSpawnPoint->GetComponentLocation(),
                //ProjectileSpawnPoint->GetComponentRotation()
                );
        }

    }
    return ProjectilePool;
}
