// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Construction_Yard.h"


ABuilding_Construction_Yard::ABuilding_Construction_Yard() {
	PrimaryActorTick.bCanEverTick = false;
	team = 1;
	maxHealth = 2000;
	currentHealth = maxHealth;
	powerUsage = 0;
	spawnTime = 0;
	cost = 100;
	isBuilding = false;

	buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConstructionYardMesh"));
	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Construction_Yard.Placeholder_Construction_Yard")).Get());
	buildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	RootComponent = buildingMesh;
	buildingMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	buildingMesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void ABuilding_Construction_Yard::BeginPlay()
{
	Super::BeginPlay();
	buildingMesh->SetMaterial(0, canBuildIndicator);
}

// Called every frame
void ABuilding_Construction_Yard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABuilding_Construction_Yard::constructAtLocation()
{
	buildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
