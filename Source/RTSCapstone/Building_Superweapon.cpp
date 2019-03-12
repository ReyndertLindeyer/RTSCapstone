// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Superweapon.h"


ABuilding_Superweapon::ABuilding_Superweapon() {
	PrimaryActorTick.bCanEverTick = true;
	//Setting up general values
	team = 1;
	spawnTime = 2;
	buildRadius = 500;
	isBuilding = true;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devTechCentre.devTechCentre")).Get());
	buildingMesh->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_Superweapon::BeginOverlap);
	buildingMesh->OnComponentEndOverlap.AddDynamic(this, &ABuilding_Superweapon::OnOverlapEnd);
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(3, buildRadius, buildRadius);

	buildingMesh->ComponentTags.Add(FName("Building"));
	decal->ComponentTags.Add(FName("BuildArea"));
}

void ABuilding_Superweapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("TURRET IS TICKING"));

}

void ABuilding_Superweapon::InitializeStructure(II_Player* player)
{
	//InitializeEntity(player, "Technology Centre", 1500.0f);
}

void ABuilding_Superweapon::SetHasPower(bool inBool)
{
	//Disable attack functions
}