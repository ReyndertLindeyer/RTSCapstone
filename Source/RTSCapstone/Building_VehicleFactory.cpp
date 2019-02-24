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

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devFactory_v1.devFactory_v1")).Get());
	buildingMesh->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_VehicleFactory::BeginOverlap);
	buildingMesh->OnComponentEndOverlap.AddDynamic(this, &ABuilding_VehicleFactory::OnOverlapEnd);
	buildingMesh->SetSimulatePhysics(false);

	//Setting up the variables from the datatable
	static ConstructorHelpers::FObjectFinderOptional<UDataTable> tempDataTable(TEXT("/Game/Game_Assets/DataTables/UnitVariables.UnitVariables"));
	unitConstructionDataTable = tempDataTable.Get();

	static const FString ContextString(TEXT("Unit Variable Context"));


	//Rifle Infantry Variables
	FUnitVariables* UnitVariables = unitConstructionDataTable->FindRow<FUnitVariables>(FName(TEXT("Harvester")), ContextString, false);
	harvesterCost = UnitVariables->Cost;
	harvesterBuildTime = UnitVariables->BuildTime;

	//Rifle Infantry Variables
	UnitVariables = unitConstructionDataTable->FindRow<FUnitVariables>(FName(TEXT("Humvee")), ContextString, false);
	humveeCost = UnitVariables->Cost;
	humveeTime = UnitVariables->BuildTime;

	//Rifle Infantry Variables
	UnitVariables = unitConstructionDataTable->FindRow<FUnitVariables>(FName(TEXT("LightTank")), ContextString, false);
	tankCost = UnitVariables->Cost;
	tankTime = UnitVariables->BuildTime;

	//Rifle Infantry Variables
	UnitVariables = unitConstructionDataTable->FindRow<FUnitVariables>(FName(TEXT("ArtillaryTank")), ContextString, false);
	artilleryTankCost = UnitVariables->Cost;
	artilleryTankBuildTime = UnitVariables->BuildTime;

	//Rifle Infantry Variables
	UnitVariables = unitConstructionDataTable->FindRow<FUnitVariables>(FName(TEXT("HeavyTank")), ContextString, false);
	heavyTankCost = UnitVariables->Cost;
	heavyTankTime = UnitVariables->BuildTime;

	//Rifle Infantry Variables
	UnitVariables = unitConstructionDataTable->FindRow<FUnitVariables>(FName(TEXT("OutpostCreator")), ContextString, false);
	outpostCost = UnitVariables->Cost;
	outpostTime = UnitVariables->BuildTime;

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


int32 ABuilding_VehicleFactory::AddToUnitQueue(int32 unitType)
{
	if (constructed) {
		PrimaryActorTick.bCanEverTick = true;
		unitQueue.Add(unitType);
		if (unitType == 1) {
			return harvesterCost;
		}
		else if (unitType == 2) {
			return humveeCost;
		}
		else if (unitType == 3) {
			return tankCost;
		}
		else if (unitType == 4) {
			return artilleryTankCost;
		}
		else if (unitType == 5) {
			return heavyTankCost;
		}
		else {
			return outpostCost;
		}
	}
	return 0;
}

int32 ABuilding_VehicleFactory::GetUnitAtStartOfQueue()
{
	if (unitQueue[0])
		return unitQueue[0];
	return 0;
}

int32 ABuilding_VehicleFactory::RemoveFromUnitQueue()
{
	if (constructed) {
		countToCompleteUnit = 0;
		if (unitQueue[0] == 1) {
			unitQueue.RemoveAt(0);
			return harvesterCost;
		}
		else if (unitQueue[0] == 2) {
			unitQueue.RemoveAt(0);
			return humveeCost;
		}
		else if (unitQueue[0] == 3) {
			unitQueue.RemoveAt(0);
			return tankCost;
		}
		else if (unitQueue[0] == 4) {
			unitQueue.RemoveAt(0);
			return artilleryTankCost;
		}
		else if (unitQueue[0] == 5) {
			unitQueue.RemoveAt(0);
			return heavyTankCost;
		}
		else {
			unitQueue.RemoveAt(0);
			return outpostCost;
		}
	}

	return 0;
}

float ABuilding_VehicleFactory::StartingTime()
{
	if (unitQueue[0] == 1) {
		return harvesterBuildTime;
	}
	else if (unitQueue[0] == 2) {
		return humveeTime;
	}
	else if (unitQueue[0] == 3) {
		return tankTime;
	}
	else if (unitQueue[0] == 4) {
		return artilleryTankBuildTime;
	}
	else if (unitQueue[0] == 5) {
		return heavyTankTime;
	}
	else {
		return outpostTime;
	}
}

float ABuilding_VehicleFactory::TimeRemaining()
{
	return countToCompleteUnit;
}

void ABuilding_VehicleFactory::SpawnUnit()
{
	//Spawn the unit and give it its information
	constructingUnit = false; 
	unitQueue[0];
	unitQueue.RemoveAt(0);
}

void ABuilding_VehicleFactory::SetWaypoint(FVector inVec) {
	wayPoint = inVec;
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
		SpawnUnit();
	}

	if (!constructingUnit && unitQueue.Num() == 0) {
		PrimaryActorTick.bCanEverTick = false;
	}
}