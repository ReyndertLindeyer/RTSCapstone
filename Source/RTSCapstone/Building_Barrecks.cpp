// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Barrecks.h"

ABuilding_Barrecks::ABuilding_Barrecks() {
	//Setting up general values
	team = 1;
	spawnTime = 2;
	buildRadius = 500;
	isBuilding = true;
	hasPower = true;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devBarracks_v1.devBarracks_v1")).Get());
	buildingMesh->SetSimulatePhysics(false);

	//Setting up the variables from the datatable
	static ConstructorHelpers::FObjectFinderOptional<UDataTable> tempDataTable(TEXT("/Game/Game_Assets/DataTables/UnitVariables.UnitVariables"));
	unitConstructionDataTable = tempDataTable.Get();

	static const FString ContextString(TEXT("Unit Variable Context"));


	//Rifle Infantry Variables
	FUnitVariables* UnitVariables = unitConstructionDataTable->FindRow<FUnitVariables>(FName(TEXT("RifleInfantry")), ContextString, false);
	rifleInfantryCost = UnitVariables->Cost;
	rifleBuildTime = UnitVariables->BuildTime;

	//Rocket Infantry Variables
	UnitVariables = unitConstructionDataTable->FindRow<FUnitVariables>(FName(TEXT("RocketInfantry")), ContextString, false);
	rocketInfantryCost = UnitVariables->Cost;
	rocketBuildTime = UnitVariables->BuildTime;

	//Engineer Variables
	UnitVariables = unitConstructionDataTable->FindRow<FUnitVariables>(FName(TEXT("Engineer")), ContextString, false);
	engineerBuildTime = UnitVariables->Cost;
	engineerBuildTime = UnitVariables->BuildTime;

	wayPoint = buildingMesh->RelativeLocation + FVector(0.0f, 100.0f, 0.0f); //Creates a waypoint 100 units in front of the barracks

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
	decal->ComponentTags.Add(FName("BuildArea"));
}

void ABuilding_Barrecks::BeginPlay()
{
	Super::BeginPlay();
	waypointMesh->SetHiddenInGame(true);
	wayPoint = buildingMesh->RelativeLocation + FVector(0.0f, 100.0f, 0.0f); //Updates Waypoint
	waypointMesh->SetRelativeLocation(wayPoint);

	if (setPlayerOwner != nullptr)
		InitializeStructure(Cast<II_Player>(setPlayerOwner), "Barracks", 700.0f);

}

int32 ABuilding_Barrecks::AddToUnitQueue(int32 unitType)
{
	if (constructed) {
		unitQueue.Add(unitType);
		if (unitType == 1) {
			return rifleInfantryCost;
		}
		else if (unitType == 2) {
			return rocketInfantryCost;
		}
		else if (unitType == 3) {
			return engineerCost;
		}
	}
	return 0;
}

int32 ABuilding_Barrecks::RemoveFromUnitQueue()
{
	if (constructed && unitQueue.Num() > 0) {
		countToCompleteUnit = 0;
		constructingUnit = !constructingUnit;
		if (unitQueue[0] == 1) {
			unitQueue.RemoveAt(0);
			return rifleInfantryCost;
		}
		else if (unitQueue[0] == 2) {
			unitQueue.RemoveAt(0);
			return rocketInfantryCost;
		}
		else {
			unitQueue.RemoveAt(0);
			return engineerCost;
		}
	}
	return 0;
}

int32 ABuilding_Barrecks::GetUnitAtStartOfQueue()
{
	if(unitQueue.Num() > 0)
		return unitQueue[0];
	return 0;
}

float ABuilding_Barrecks::StartingTime()
{
	if (unitQueue[0] == 1) {
		return rifleBuildTime;
	}
	else if (unitQueue[0] == 2) {
		return rocketBuildTime;
	}
	else {
		return engineerBuildTime;
	}
}

float ABuilding_Barrecks::TimeRemaining()
{
	return countToCompleteUnit;
}

void ABuilding_Barrecks::SpawnUnit()
{
	UWorld* const World = this->GetWorld();
	ACharacter* holder;
	//Spawn the unit and give it its information
	constructingUnit = false;
	if (unitQueue[0] == 1) {
		holder = World->SpawnActor<AUNIT_Rifleman>(AUNIT_Rifleman::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 100.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
		Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "Rifleman", 200.0f);
	}
	else if (unitQueue[0] == 2) {
		holder = World->SpawnActor<AUNIT_Rocketeer>(AUNIT_Rocketeer::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 100.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
		Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "Rocketeer", 200.0f);
	}
	else {
		holder = World->SpawnActor<AUNIT_Engineer>(AUNIT_Engineer::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 100.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
		Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "Engineer", 200.0f);
	}
	Cast<II_Unit>(holder)->MoveOrder(holder->GetController(), wayPoint);
	unitQueue.RemoveAt(0);
}

void ABuilding_Barrecks::SetWaypoint(FVector inVec) {
	wayPoint = inVec;
	waypointMesh->SetWorldLocation(inVec);
}

void ABuilding_Barrecks::SetHasPower(bool inBool)
{
	hasPower = inBool;
}

void ABuilding_Barrecks::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!constructingUnit && unitQueue.Num() > 0 && constructed) {
		if (unitQueue[0] == 1) {
			countToCompleteUnit = rifleBuildTime;
		}
		else if (unitQueue[0] == 2) {
			countToCompleteUnit = rocketBuildTime;
		}
		else if (unitQueue[0] == 3) {
			countToCompleteUnit = engineerBuildTime;
		}
		constructingUnit = true;
	}

	if (constructingUnit) {
		//If the structure has enough power then construct normally, if not then construct at half progress
		if(hasPower)
			countToCompleteUnit -= DeltaTime;
		else
			countToCompleteUnit -= DeltaTime/2;
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
