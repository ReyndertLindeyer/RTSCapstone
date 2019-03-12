// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Turret_Tesla.h"

ABuilding_Turret_Tesla::ABuilding_Turret_Tesla() {
	spawnTime = 2;
	isBuilding = true;
	isPlaced = false;
	hasPower = true;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Engine/BasicShapes/Cylinder.Cylinder")).Get());
	buildingMesh->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_Turret_Tesla::BeginOverlap);
	buildingMesh->OnComponentEndOverlap.AddDynamic(this, &ABuilding_Turret_Tesla::OnOverlapEnd);
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);
}

void ABuilding_Turret_Tesla::InitializeStructure(II_Player* player)
{
	InitializeEntity(player, "Turret", 1200.0f);
}

void ABuilding_Turret_Tesla::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("TURRET IS TICKING"));

}

void ABuilding_Turret_Tesla::SetHasPower(bool inBool)
{
	//Disable attack functions
}


