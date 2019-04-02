// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_PlanetaryCannon.h"

ABuilding_PlanetaryCannon::ABuilding_PlanetaryCannon() {
	PrimaryActorTick.bCanEverTick = true;
	//Setting up general values
	team = 1;
	spawnTime = 2;
	buildRadius = 500;
	isBuilding = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BodyMeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Planetery_Cannon_Model/PlanetaryCannon_v1.PlanetaryCannon_v1'"));
	UStaticMesh* bodyMesh = BodyMeshAsset.Object;
	buildingMesh->SetStaticMesh(bodyMesh);
	buildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	buildingMesh->SetRelativeScale3D(FVector(10.0f));
	buildingMesh->SetCanEverAffectNavigation(false);
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(3, buildRadius, buildRadius);

	buildingMesh->ComponentTags.Add(FName("Building"));
}

// Called when the game starts or when spawned
void ABuilding_PlanetaryCannon::BeginPlay()
{
	Super::BeginPlay();

	buildingMesh->SetWorldScale3D(FVector(10));
}

void ABuilding_PlanetaryCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (setPlayerOwner != nullptr) {
		InitializeStructure(Cast<II_Player>(setPlayerOwner), "Planetary Cannon", 100.0f);
		setPlayerOwner = nullptr;
		return;
	}

}