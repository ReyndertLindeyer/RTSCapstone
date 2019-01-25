// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Barrecks.h"
#include "RifleInfantry.h"

ABuilding_Barrecks::ABuilding_Barrecks() {
	PrimaryActorTick.bCanEverTick = false;
	//Setting up general values
	team = 1;
	maxHealth = 500;
	currentHealth = maxHealth;
	powerUsage = 10;
	spawnTime = 2;
	cost = 100;
	isBuilding = true;

	buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarracksMesh"));
	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Barracks.Placeholder_Barracks")).Get());
	buildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	RootComponent = buildingMesh;
	buildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	buildingMesh->SetSimulatePhysics(false);

	//Setting up specific values
	rifleBuildTime = 10;
	rifleInfantryCost = 20;
	rocketBuildTime = 10;
	rocketInfantryCost = 40;
	engineerBuildTime = 40;
	engineerCost = 80;

	countToCompleteUnit = 0.0f;

	constructingUnit = false;
}

void ABuilding_Barrecks::BuildRiflemen()
{
	if (!isBuilding) {
		constructingUnit = true;
		countToCompleteUnit = rifleBuildTime;
	}
}

void ABuilding_Barrecks::BuildRocketeer()
{
	if (!isBuilding) {
		constructingUnit = true;
		countToCompleteUnit = rocketBuildTime;
	}
}

void ABuilding_Barrecks::BuildEngineer()
{
	if (!isBuilding) {
		constructingUnit = true;
		countToCompleteUnit = engineerBuildTime;
	}
}

void ABuilding_Barrecks::AddToUnitQueue(int unitType)
{
	PrimaryActorTick.bCanEverTick = true;
	unitQueue.Add(unitType);
}

void ABuilding_Barrecks::SpawnUnit(int unitType)
{
	//Spawn the unit and information
	constructingUnit = false;
	unitQueue.RemoveAt(0);
}

void ABuilding_Barrecks::BeginPlay()
{
	Super::BeginPlay();

}

void ABuilding_Barrecks::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!constructingUnit && unitQueue.Num() > 0) {
		if (unitQueue[0] == 1) {
			BuildRiflemen();
		}
		else if (unitQueue[0] == 2) {
			BuildRocketeer();
		}
		else if (unitQueue[0] == 3) {
			BuildEngineer();
		}
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