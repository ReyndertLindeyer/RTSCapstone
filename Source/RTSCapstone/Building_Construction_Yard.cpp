// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Construction_Yard.h"


ABuilding_Construction_Yard::ABuilding_Construction_Yard() {
	PrimaryActorTick.bCanEverTick = true;
	team = 1;
	maxHealth = 2000;
	currentHealth = maxHealth;
	powerUsage = 0;
	spawnTime = 0;
	buildRadius = 100;
	cost = 100;
	isBuilding = false;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Construction_Yard.Placeholder_Construction_Yard")).Get());
	RootComponent = buildingMesh;
	buildingMesh->SetSimulatePhysics(false);
	
	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);
	
	buildRadiusSphere->SetSphereRadius(buildRadius);
	buildRadiusSphere->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_Construction_Yard::BeginRadiusOverlap);
	buildRadiusSphere->OnComponentEndOverlap.AddDynamic(this, &ABuilding_Construction_Yard::OnRadiusOverlapEnd);
	buildRadiusSphere->SetupAttachment(RootComponent);

	buildingMesh->ComponentTags.Add(FName("Building"));
	buildRadiusSphere->ComponentTags.Add(FName("buildRadius"));
	decal->ComponentTags.Add(FName("BuildArea"));
}

// Called when the game starts or when spawned
void ABuilding_Construction_Yard::BeginPlay()
{
	Super::BeginPlay();
	buildingMesh->SetMaterial(0, regularMaterial);
	constructed = true;
	isPlaced = true;
}

// Called every frame
void ABuilding_Construction_Yard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}