// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Construction_Yard.h"


ABuilding_Construction_Yard::ABuilding_Construction_Yard() {
	PrimaryActorTick.bCanEverTick = true;
	team = 1;
	spawnTime = 0;
	buildRadius = 10000;
	isBuilding = false;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/ConstructionYard_Model/Construction_Yard.Construction_Yard")).Get());
	RootComponent = buildingMesh;
	buildingMesh->SetSimulatePhysics(false);
	
	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(10, buildRadius, buildRadius);

	buildingMesh->ComponentTags.Add(FName("Building"));
}

// Called when the game starts or when spawned
void ABuilding_Construction_Yard::BeginPlay()
{
	Super::BeginPlay();
	constructed = true;
	isBuilding = false;

	if (setPlayerOwner != nullptr)
		InitializeStructure(Cast<II_Player>(setPlayerOwner), "Construction Yard", 20000.0f);
}

// Called every frame
void ABuilding_Construction_Yard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}