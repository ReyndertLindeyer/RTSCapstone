// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Turret.h"
#include "ConstructorHelpers.h"

ABuilding_Turret::ABuilding_Turret() {
	spawnTime = 2;
	isBuilding = true;
	isPlaced = false;
	hasPower = true;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Engine/BasicShapes/Cylinder.Cylinder")).Get());
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);
}

void ABuilding_Turret::InitializeStructure(II_Player* player)
{
	InitializeEntity(player, "Turret", 1200.0f);
}

void ABuilding_Turret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//UE_LOG(LogTemp, Warning, TEXT("TURRET IS TICKING"));

}

void ABuilding_Turret::SetHasPower(bool inBool)
{
	//Disable attack functions
}
