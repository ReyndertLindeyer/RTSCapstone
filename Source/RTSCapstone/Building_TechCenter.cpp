// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_TechCenter.h"

ABuilding_TechCenter::ABuilding_TechCenter() {
	PrimaryActorTick.bCanEverTick = true;
	//Setting up general values
	team = 1;
	maxHealth = 500;
	currentHealth = maxHealth;
	powerUsage = 50;
	spawnTime = 2;
	cost = 100;
	buildRadius = 500;
	isBuilding = true;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devTechCentre.devTechCentre")).Get());
	buildingMesh->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_TechCenter::BeginOverlap);
	buildingMesh->OnComponentEndOverlap.AddDynamic(this, &ABuilding_TechCenter::OnOverlapEnd);
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(3, buildRadius, buildRadius);

	buildRadiusSphere->SetSphereRadius(10);
	buildRadiusSphere->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_TechCenter::BeginRadiusOverlap);
	buildRadiusSphere->OnComponentEndOverlap.AddDynamic(this, &ABuilding_TechCenter::OnRadiusOverlapEnd);
	buildRadiusSphere->SetupAttachment(RootComponent);

	buildingMesh->ComponentTags.Add(FName("Building"));
	buildRadiusSphere->ComponentTags.Add(FName("buildRadius"));
	decal->ComponentTags.Add(FName("BuildArea"));
}