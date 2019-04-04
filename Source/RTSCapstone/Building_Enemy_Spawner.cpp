// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Enemy_Spawner.h"
#include "Enemy_BaseManager.h"

ABuilding_Enemy_Spawner::ABuilding_Enemy_Spawner() {

	PrimaryActorTick.bCanEverTick = false;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/ESpawner_Models/BasicEnemySpawner.BasicEnemySpawner")).Get());
	buildingMesh->SetSimulatePhysics(false);
	buildingMesh->SetRelativeScale3D(FVector(4.0f));

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);

	spawnPt = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	spawnPt->SetRelativeLocation(FVector(-90.0, 0.0f, 25.0f));

	waypoint = buildingMesh->RelativeLocation + FVector(0.0f, 100.0f, 0.0f); //Creates a waypoint 100 units in front of the barracks
	

	buildingType = 1;
	

	static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("Class'/Game/Game_Assets/Blueprints/BarracksBlowingUp.BarracksBlowingUp_C'"));
	if (ItemBlueprint.Object) {
		ExplosionBlueprint = (UClass*)ItemBlueprint.Object;
	}
}


void ABuilding_Enemy_Spawner::BeginPlay()
{
	Super::BeginPlay();

	constructed = true;

	if (setPlayerOwner != nullptr) {
		InitializeStructure(Cast<II_Player>(setPlayerOwner), "Spawner", 100.0f);
	}
}

void ABuilding_Enemy_Spawner::SpawnUnit()
{
	UWorld* const World = this->GetWorld();
	ACharacter* holder;
	if (buildingType == 1) {
		holder = World->SpawnActor<AUNIT_Grinder>(AUNIT_Grinder::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 400.0f, 110.0f), FRotator(0.0f, 0.0f, 0.0f));
		Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "Grinder", 1250.0f);
		Cast<II_Unit>(holder)->MoveOrder(holder->GetController(), waypoint);
	}
	else if (buildingType == 2) {
		holder = World->SpawnActor<AUNIT_Gattling>(AUNIT_Gattling::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 400.0f, 110.0f), FRotator(0.0f, 0.0f, 0.0f));
		Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "Gattling", 850.0f);
		Cast<II_Unit>(holder)->MoveOrder(holder->GetController(), waypoint);
	}
	else if (buildingType == 3) {
		holder = World->SpawnActor<AUNIT_Roomba>(AUNIT_Roomba::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 400.0f, 110.0f), FRotator(0.0f, 0.0f, 0.0f));
		Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "Roomba", 1000.0f);
		Cast<II_Unit>(holder)->MoveOrder(holder->GetController(), waypoint);
	}
	else if (buildingType == 4) {
		holder = World->SpawnActor<AUNIT_Prism>(AUNIT_Prism::StaticClass(), buildingMesh->RelativeLocation + FVector(0.0f, 400.0f, 110.0f), FRotator(0.0f, 0.0f, 0.0f));
		Cast<II_Entity>(holder)->InitializeEntity(GetEntityOwner(), "Roomba", 1000.0f);
		Cast<II_Unit>(holder)->MoveOrder(holder->GetController(), waypoint);
	}
}

void ABuilding_Enemy_Spawner::SetupBulding(FVector inVec)
{
	waypoint = inVec;
}

void ABuilding_Enemy_Spawner::SetMesh(UStaticMesh* inMesh) {
	buildingMesh->SetStaticMesh(inMesh);
}


void ABuilding_Enemy_Spawner::DestroyEntity()
{
	// Remove from Owner's Array
	Cast<AEnemy_BaseManager>(myManager)->RemoveBuilding(this);
}

void ABuilding_Enemy_Spawner::KillMe() {

	if (GetEntityOwner() != nullptr)
	{
		if (GetEntityOwner()->GetUnits().Contains(this))
			GetEntityOwner()->GetUnits().Remove(this);

		if (GetEntityOwner()->GetBuildings().Contains(this))
			GetEntityOwner()->GetBuildings().Remove(this);
	}

	Destroy(this);
}