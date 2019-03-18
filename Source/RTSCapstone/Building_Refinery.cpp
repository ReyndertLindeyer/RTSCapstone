// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Refinery.h"
#include "UNIT_Harvester.h"

ABuilding_Refinery::ABuilding_Refinery() {
	PrimaryActorTick.bCanEverTick = true;
	//Setting up general values
	team = 1;
	spawnTime = 2;
	buildRadius = 500;
	isBuilding = true;
	canSpawnHarvester = true;

	this->Tags.Add(FName("Refinery"));
	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devRefinery_v1.devRefinery_v1")).Get());
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);

	buildingMesh->ComponentTags.Add(FName("Building"));
	decal->ComponentTags.Add(FName("BuildArea"));


	harvestPt = CreateDefaultSubobject<USceneComponent>(TEXT("Harvest Point"));
	harvestPt->SetupAttachment(RootComponent);
	harvestPt->SetRelativeLocation(FVector(-75.0f, 0.0f, 40.0f));

	isOccupied = false;
	
}

void ABuilding_Refinery::BeginPlay()
{
	Super::BeginPlay();

}

void ABuilding_Refinery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABuilding_Refinery::InitializeRefinery()
{
	FActorSpawnParameters SpawnInfo;
	//AActor* MyActor = GWorld->SpawnActor<AUNIT_Harvester>(AUNIT_Harvester::StaticClass(), harvestPt->GetRelativeTransform(), FRotator::ZeroRotator, SpawnInfo);
	AActor* spawnedHarvy = GetWorld()->SpawnActor<AUNIT_Harvester>(AUNIT_Harvester::StaticClass(), harvestPt->GetComponentLocation(), FRotator(0.0f, 0.0f, 0.0f));
	Cast<II_Entity>(spawnedHarvy)->InitializeEntity(GetEntityOwner(), "Harvester", 2000.0f);

}