// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Enemy_Spawner.h"

ABuilding_Enemy_Spawner::ABuilding_Enemy_Spawner() {

	PrimaryActorTick.bCanEverTick = false;


	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);

	waypoint = buildingMesh->RelativeLocation + FVector(0.0f, 100.0f, 0.0f); //Creates a waypoint 100 units in front of the barracks

	buildingType = 0;
}

void ABuilding_Enemy_Spawner::SpawnUnit()
{
	if (buildingType == 1) {
		///Spawn small melee unit
	}
	else if (buildingType == 2) {
		///Spawn small ranged unit
	}
	else if (buildingType == 3) {
		///Spawn large melee unit
	}
	else if (buildingType == 4) {
		///Spawn large ranged unit
	}
}

void ABuilding_Enemy_Spawner::SetupBulding(FVector inVec, UStaticMesh * inMesh, int32 type)
{
	waypoint = inVec;
	buildingMesh->SetStaticMesh(inMesh);
	buildingType = type;
}
