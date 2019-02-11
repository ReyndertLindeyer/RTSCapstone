// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_VehicleFactory.h"

ABuilding_VehicleFactory::ABuilding_VehicleFactory() {

	//Setting up general values
	team = 1;
	maxHealth = 500;
	currentHealth = maxHealth;
	powerUsage = 10;
	spawnTime = 2;
	cost = 100;
	buildRadius = 500;
	isBuilding = true;
	isPlaced = false;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devBarracks_v1.devBarracks_v1")).Get());
	buildingMesh->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_VehicleFactory::BeginOverlap);
	buildingMesh->OnComponentEndOverlap.AddDynamic(this, &ABuilding_VehicleFactory::OnOverlapEnd);
	buildingMesh->SetSimulatePhysics(false);

	//Setting up specific values
	harvesterBuildTime = 10;
	harvesterCost = 300;

	humveeTime = 4;
	humveeCost = 100;

	tankTime = 8;
	tankCost = 200;

	artilleryTankBuildTime = 8;
	artilleryTankCost = 300;

	heavyTankTime = 10;
	heavyTankCost = 500;
	
	outpostTime = 5;
	outpostCost = 200;

	wayPoint = buildingMesh->RelativeLocation + FVector(0.0f, 100.0f, 0.0f); //Creates a waypoint 100 units in front of the barracks

	countToCompleteUnit = 0.0f;

	constructingUnit = false;

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);

	buildRadiusSphere->SetSphereRadius(10);
	buildRadiusSphere->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_VehicleFactory::BeginRadiusOverlap);
	buildRadiusSphere->OnComponentEndOverlap.AddDynamic(this, &ABuilding_VehicleFactory::OnRadiusOverlapEnd);
	buildRadiusSphere->SetupAttachment(RootComponent);

	buildingMesh->ComponentTags.Add(FName("Building"));
	buildRadiusSphere->ComponentTags.Add(FName("buildRadius"));
	decal->ComponentTags.Add(FName("BuildArea"));
}


void ABuilding_VehicleFactory::AddToUnitQueue(int unitType)
{
	if (constructed) {
		PrimaryActorTick.bCanEverTick = true;
		unitQueue.Add(unitType);
	}
}

void ABuilding_VehicleFactory::SpawnUnit(int unitType)
{
	//Spawn the unit and give it its information
	constructingUnit = false;
	unitQueue.RemoveAt(0);
}

void ABuilding_VehicleFactory::SetWaypoint(FVector inVec) {
	wayPoint = inVec;
}

uint8 ABuilding_VehicleFactory::GetUnitCost(uint8 whatUnit)
{
	if (whatUnit == 1) {
		return harvesterCost;
	}
	else if (whatUnit == 2) {
		return humveeCost;
	}
	else if (whatUnit == 3) {
		return tankCost;
	}
	else if (whatUnit == 4) {
		return artilleryTankCost;
	}
	else if (whatUnit == 5) {
		return heavyTankCost;
	}
	else {
		return outpostCost;
	}
}

void ABuilding_VehicleFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!constructingUnit && unitQueue.Num() > 0 && constructed) {
		if (unitQueue[0] == 1) {
			countToCompleteUnit = harvesterBuildTime;
		}
		else if (unitQueue[0] == 2) {
			countToCompleteUnit = humveeTime;
		}
		else if (unitQueue[0] == 3) {
			countToCompleteUnit = tankTime;
		}
		else if (unitQueue[0] == 4) {
			countToCompleteUnit = artilleryTankBuildTime;
		}
		else if (unitQueue[0] == 5) {
			countToCompleteUnit = heavyTankTime;
		}
		else if (unitQueue[0] == 6) {
			countToCompleteUnit = outpostTime;
		}
		constructingUnit = true;
	}

	if (constructingUnit) {
		countToCompleteUnit -= DeltaTime;
	}

	if (constructingUnit && countToCompleteUnit < 0.0f) {
		SpawnUnit(unitQueue[0]);
	}

	if (!constructingUnit && unitQueue.Num() == 0) {
		PrimaryActorTick.bCanEverTick = false;
	}
}