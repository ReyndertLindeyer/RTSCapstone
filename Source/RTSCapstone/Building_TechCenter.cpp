// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_TechCenter.h"

ABuilding_TechCenter::ABuilding_TechCenter() {
	PrimaryActorTick.bCanEverTick = true;
	//Setting up general values
	team = 1;
	spawnTime = 2;
	buildRadius = 500;
	isBuilding = true;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devTechCentre.devTechCentre")).Get());
	buildingMesh->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_TechCenter::BeginOverlap);
	buildingMesh->OnComponentEndOverlap.AddDynamic(this, &ABuilding_TechCenter::OnOverlapEnd);
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(3, buildRadius, buildRadius);

	buildingMesh->ComponentTags.Add(FName("Building"));
	decal->ComponentTags.Add(FName("BuildArea"));
}

void ABuilding_TechCenter::InitializeStructure(II_Player* player)
{
	InitializeEntity(player, "Technology Centre", 1500.0f);
}