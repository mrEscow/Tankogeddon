// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsComponent.h"

TArray<FVector> UPhysicsComponent::GenerateTrajectory(FVector StartPos, FVector Velocity, float MaxTime, float TimeStep, float MinZValue)
{
	TArray<FVector> trajectory;

	FVector gravityVector(0, 0, Gravity);

	for (float time = 0; time < MaxTime; time = time + TimeStep)
	{
		FVector position = 

			StartPos + // начальная точка
			(Velocity * time) + // вектор направления
			(gravityVector * time * time / 2);  // гравитация

		if (position.Z <= MinZValue)
		{
			break;
		}

		trajectory.Add(position);
	}

	return trajectory;
}

TArray<FVector> UPhysicsComponent::GenerateTrajectoryBezie2P(FVector StartPos, FVector EndPos, float Time, float TimeStep)
{
	TArray<FVector> Trajectory;

	FVector P1 = (StartPos + EndPos) / 2;
	P1.Z = (StartPos.Z + EndPos.Z) * 3;

	for (float time = 0; time < Time; time = time + TimeStep)
	{
		FVector PointPosition = GetPosBezie2P(StartPos, P1, EndPos, time / Time);

		Trajectory.Add(PointPosition);
	}


	return Trajectory;
}

FVector UPhysicsComponent::GetPosBezie2P(FVector P0, FVector P1, FVector P2, float Time)
{
	// B(t) = (1-t)2P0 + 2(1-t)tP1 + t2P2
	// return  uu            u        tt

	float u = 1 - Time;
	float tt = Time * Time;
	float uu = u * u;

	FVector PointPosition = (uu * P0) + ((2 * u) * Time * P1) + (tt * P2);

	return PointPosition;
}