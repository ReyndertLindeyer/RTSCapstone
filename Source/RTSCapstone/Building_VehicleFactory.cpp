// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_VehicleFactory.h"

ABuilding_VehicleFactory::ABuilding_VehicleFactory() {
	//Setting up general values
	team = 1;
	spawnTime = 2;
	buildRadius = 500;
	isBuilding = true;
	hasPower = true;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devFactory_v1.devFactory_v1")).Get());
	buildingMesh->SetSimulatePhysics(false);

	waypointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("waypointMesh"));
	waypointMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Waypoint.Waypoint")).Get());
	waypointMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	waypointMesh->SetSimulatePhysics(false);
	waypointMesh->SetupAttachment(RootComponent); 

	countToCompleteUnit = 0.0f;

	constructingUnit = false;

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);

	buildingMesh->ComponentTags.Add(FName("Building"));
}

void ABuilding_VehicleFactory::BeginPlay()
{
	Super::BeginPlay();
	waypointMesh->SetHiddenInGame(true);

	if (setPlayerOwner != nullptr) {
		InitializeStructure(Cast<II_Player>(setPlayerOwner), "Placeholder", 10.0f);
		SetName(TEXT("WarFactory"));
		SetMaxHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("WarFactory")), (TEXT("Context")), false)->MaxHealth);
		SetCurrentHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("WarFactory")), (TEXT("Context")), false)->MaxHealth);
	}

	buildingMesh->SetWorldScale3D(FVector(6));

	wayPoint = GetActorLocation() + FVector(0.0f, 500.0f, 0.0f);
	waypointMesh->SetWorldLocation(wayPoint);
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
	if (unitQueue.Num() > 0)
		return unitQueue[0];
	return 0;
}

int32 ABuilding_VehicleFactory::RemoveFromUnitQueue()
{
	if (constructed && unitQueue.Num() > 0) {
		constructingUnit = !constructingUnit;
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
	UWorld* const World = this->GetWorld();
	ACharacter* holder;
	//Spawn the unit and give it its information
	constructingUnit = false;
	if (unitQueue[0] == 3) {
		holder = World->SpawnActor<AUNIT_Harvester>(AUNIT_Harvester::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 500.0f, 200.0f), FRotator(0.0f, 0.0f, 0.0f));
		Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "Harvester", 200.0f);
	}
	else if (unitQueue[0] == 1) {
		holder = World->SpawnActor<AUNIT_Scout>(AUNIT_Scout::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 500.0f, 200.0f), FRotator(0.0f, 0.0f, 0.0f));
		Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "Humvee", 200.0f);
	}
	else  if (unitQueue[0] == 2) {
		holder = World->SpawnActor<AUNIT_MBT>(AUNIT_MBT::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 500.0f, 200.0f), FRotator(0.0f, 0.0f, 0.0f));
		Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "Tank", 200.0f);
	}
	else if (unitQueue[0] == 4) {
	holder = World->SpawnActor<AUNIT_MArtillery>(AUNIT_MArtillery::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 500.0f, 200.0f), FRotator(0.0f, 0.0f, 0.0f));
	Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "Artillery", 200.0f);
	}
	else if (unitQueue[0] == 5) {
	holder = World->SpawnActor<AUNIT_AvBT>(AUNIT_AvBT::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 500.0f, 200.0f), FRotator(0.0f, 0.0f, 0.0f));
	Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "HeavyTank", 200.0f);
	}
	else {
	holder = World->SpawnActor<AUNIT_MOutpost>(AUNIT_MOutpost::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 500.0f, 200.0f), FRotator(0.0f, 0.0f, 0.0f));
	Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "Outpost", 200.0f);
	}
	Cast<II_Unit>(holder)->MoveOrder(holder->GetController(), wayPoint);
	unitQueue.RemoveAt(0);
}

void ABuilding_VehicleFactory::SetWaypoint(FVector inVec) {
	wayPoint = inVec;
	waypointMesh->SetWorldLocation(inVec);
}

void ABuilding_VehicleFactory::SetHasPower(bool inBool)
{
	hasPower = inBool;
}

void ABuilding_VehicleFactory::InitializeWarFactory()
{

	wayPoint = GetActorLocation() + FVector(0.0f, 500.0f, 0.0f); //Updates Waypoint
	waypointMesh->SetWorldLocation(wayPoint);
	static const FString ContextString(TEXT("Unit Variable Context"));
	//Harvester Variables
	FUnitVariables* UnitVariables = GetEntityOwner()->GetUnitConstructionDataTable()->FindRow<FUnitVariables>(FName(TEXT("Harvester")), ContextString, false);
	harvesterCost = UnitVariables->Cost;
	harvesterBuildTime = UnitVariables->BuildTime;

	//Humvee Variables
	UnitVariables = GetEntityOwner()->GetUnitConstructionDataTable()->FindRow<FUnitVariables>(FName(TEXT("Humvee")), ContextString, false);
	humveeCost = UnitVariables->Cost;
	humveeTime = UnitVariables->BuildTime;

	//Light Tank Variables
	UnitVariables = GetEntityOwner()->GetUnitConstructionDataTable()->FindRow<FUnitVariables>(FName(TEXT("LightTank")), ContextString, false);
	tankCost = UnitVariables->Cost;
	tankTime = UnitVariables->BuildTime;

	//Artillary Tank Variables
	UnitVariables = GetEntityOwner()->GetUnitConstructionDataTable()->FindRow<FUnitVariables>(FName(TEXT("ArtillaryTank")), ContextString, false);
	artilleryTankCost = UnitVariables->Cost;
	artilleryTankBuildTime = UnitVariables->BuildTime;

	//Heavy Tank Variables
	UnitVariables = GetEntityOwner()->GetUnitConstructionDataTable()->FindRow<FUnitVariables>(FName(TEXT("HeavyTank")), ContextString, false);
	heavyTankCost = UnitVariables->Cost;
	heavyTankTime = UnitVariables->BuildTime;

	//Outpost Creator Variables
	UnitVariables = GetEntityOwner()->GetUnitConstructionDataTable()->FindRow<FUnitVariables>(FName(TEXT("OutpostCreator")), ContextString, false);
	outpostCost = UnitVariables->Cost;
	outpostTime = UnitVariables->BuildTime;
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
		//If the structure has enough power then construct normally, if not then construct at half progress
		if (hasPower)
			countToCompleteUnit -= DeltaTime;
		else
			countToCompleteUnit -= DeltaTime / 2;
	}

	if (constructingUnit && countToCompleteUnit < 0.0f) {
		SpawnUnit();
	}

	if (!constructingUnit && unitQueue.Num() == 0) {
		PrimaryActorTick.bCanEverTick = false;
	}

	if (IsSelected() && waypointMesh->bHiddenInGame) {
		waypointMesh->SetHiddenInGame(false);
	}
	else if (!IsSelected() && !waypointMesh->bHiddenInGame) {
		waypointMesh->SetHiddenInGame(true);
	}
}