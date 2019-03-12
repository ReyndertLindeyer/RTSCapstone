// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Turret_Artillery.h"

ABuilding_Turret_Artillery::ABuilding_Turret_Artillery() {
	spawnTime = 2;
	isBuilding = true;
	isPlaced = false;
	hasPower = true;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Engine/BasicShapes/Cylinder.Cylinder")).Get());
	buildingMesh->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_Turret_Artillery::BeginOverlap);
	buildingMesh->OnComponentEndOverlap.AddDynamic(this, &ABuilding_Turret_Artillery::OnOverlapEnd);
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);
}

void ABuilding_Turret_Artillery::InitializeStructure(II_Player* player)
{
	InitializeEntity(player, "Turret", 1200.0f);
}

void ABuilding_Turret_Artillery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("TURRET IS TICKING"));

}

void ABuilding_Turret_Artillery::SetHasPower(bool inBool)
{
	//Disable attack functions
}