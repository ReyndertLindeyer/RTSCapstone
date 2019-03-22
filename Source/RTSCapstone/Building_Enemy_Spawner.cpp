// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Enemy_Spawner.h"

ABuilding_Enemy_Spawner::ABuilding_Enemy_Spawner() {

	PrimaryActorTick.bCanEverTick = false;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere.Sphere")).Get());
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);

	waypoint = buildingMesh->RelativeLocation + FVector(0.0f, 100.0f, 0.0f); //Creates a waypoint 100 units in front of the barracks

	buildingType = 1;
}


void ABuilding_Enemy_Spawner::BeginPlay()
{
	Super::BeginPlay();

	constructed = true;

	if (setPlayerOwner != nullptr)
		InitializeStructure(Cast<II_Player>(setPlayerOwner), "Spawner", 10.0f);
}

void ABuilding_Enemy_Spawner::SpawnUnit()
{
	UWorld* const World = this->GetWorld();
	ACharacter* holder;
	if (buildingType == 1) {
		holder = World->SpawnActor<AUNIT_Rifleman>(AUNIT_Rifleman::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 100.0f, 110.0f), FRotator(0.0f, 0.0f, 0.0f));
		Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "Rifleman", 200.0f);
		Cast<II_Unit>(holder)->MoveOrder(holder->GetController(), waypoint);
	}
	else if (buildingType == 2) {
		holder = World->SpawnActor<AUNIT_Rocketeer>(AUNIT_Rocketeer::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 100.0f, 110.0f), FRotator(0.0f, 0.0f, 0.0f));
		Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "Rocketeer", 200.0f);
		Cast<II_Unit>(holder)->MoveOrder(holder->GetController(), waypoint);
	}
	else if (buildingType == 3) {
		holder = World->SpawnActor<AUNIT_Engineer>(AUNIT_Engineer::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 100.0f, 110.0f), FRotator(0.0f, 0.0f, 0.0f));
		Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "Engineer", 200.0f);
		Cast<II_Unit>(holder)->MoveOrder(holder->GetController(), waypoint);
	}
	else if (buildingType == 4) {
		///Spawn large ranged unit
	}
}

void ABuilding_Enemy_Spawner::SetupBulding(FVector inVec)
{
	waypoint = inVec;
}

void ABuilding_Enemy_Spawner::SetMesh(UStaticMesh* inMesh) {
	buildingMesh->SetStaticMesh(inMesh);
}