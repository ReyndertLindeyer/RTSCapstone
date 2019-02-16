// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Enemy_Spawner.h"

ABuilding_Enemy_Spawner::ABuilding_Enemy_Spawner() {

	PrimaryActorTick.bCanEverTick = false;

	buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	buildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	RootComponent = buildingMesh;
	buildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	buildingMesh->SetCollisionProfileName(TEXT("Trigger"));

	maxHealth = 10;
	currentHealth = 10;

	waypoint = buildingMesh->RelativeLocation + FVector(0.0f, 100.0f, 0.0f); //Creates a waypoint 100 units in front of the barracks
}

void ABuilding_Enemy_Spawner::SpawnUnit(int unitType)
{
	if (type == 1) {
		///Spawn small melee unit
	}
	else if (type == 2) {
		///Spawn small ranged unit
	}
	else if (type == 3) {
		///Spawn large melee unit
	}
	else if (type == 4) {
		///Spawn large ranged unit
	}
}

void ABuilding_Enemy_Spawner::SetBuldingType(FVector inVec, UStaticMesh * inMesh, int32 buildingType)
{
	waypoint = inVec;
	buildingMesh->SetStaticMesh(inMesh);
	type = buildingType;
}
