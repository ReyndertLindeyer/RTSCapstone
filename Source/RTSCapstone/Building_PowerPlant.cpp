// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_PowerPlant.h"

ABuilding_PowerPlant::ABuilding_PowerPlant() {
	PrimaryActorTick.bCanEverTick = false;
	//Setting up general values
	team = 1;
	maxHealth = 500;
	currentHealth = maxHealth;
	powerUsage = -50;
	spawnTime = 2;
	cost = 100;
	isBuilding = true;

	buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerPlantMesh"));
	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Power_Plant.Placeholder_Power_Plant")).Get());
	buildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	RootComponent = buildingMesh;
	buildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	buildingMesh->SetCollisionProfileName(TEXT("Trigger"));
	buildingMesh->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_PowerPlant::BeginOverlap);
	buildingMesh->OnComponentEndOverlap.AddDynamic(this, &ABuilding_PowerPlant::OnOverlapEnd);
	buildingMesh->SetSimulatePhysics(false);
}

void ABuilding_PowerPlant::Upgrade()
{
	///Set new mesh
	/*
	buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerPlantMesh"));
	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Power_Plant.Placeholder_Power_Plant")).Get());
	buildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	RootComponent = buildingMesh;
	buildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	buildingMesh->SetSimulatePhysics(false);
	*/
}
