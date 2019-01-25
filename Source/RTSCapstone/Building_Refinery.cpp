// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Refinery.h"

ABuilding_Refinery::ABuilding_Refinery() {
	PrimaryActorTick.bCanEverTick = false;
	//Setting up general values
	team = 1;
	maxHealth = 500;
	currentHealth = maxHealth;
	powerUsage = 20;
	spawnTime = 2;
	cost = 100;
	isBuilding = true;

	buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RefineryMesh"));
	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Refinery.Placeholder_Refinery")).Get());
	buildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	RootComponent = buildingMesh;
	buildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	buildingMesh->SetSimulatePhysics(false);

	//Spawn a harvester
}