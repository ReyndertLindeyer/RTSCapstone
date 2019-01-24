// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Barrecks.h"

ABuilding_Barrecks::ABuilding_Barrecks() {
	//Setting up general values
	team = 1;
	maxHealth = 500;
	currentHealth = maxHealth;
	powerUsage = 10;
	spawnTime = 2;
	cost = 100;
	isBuilding = true;

	buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrecksMesh"));
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
	harvesterBuildTime = 40;
	harvesterCost = 80;
}

void ABuilding_Barrecks::BeginPlay()
{
	Super::BeginPlay();

}