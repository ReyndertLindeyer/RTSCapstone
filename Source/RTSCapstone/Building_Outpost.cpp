// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Outpost.h"


ABuilding_Outpost::ABuilding_Outpost() {
	PrimaryActorTick.bCanEverTick = false;
	//Setting up general values
	spawnTime = 2;
	buildRadius = 5000;
	isBuilding = true;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Construction_Yard.Placeholder_Construction_Yard")).Get());
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(10, buildRadius, buildRadius);
}


void ABuilding_Outpost::BeginPlay()
{
	Super::BeginPlay();

}