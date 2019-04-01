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
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(3, buildRadius, buildRadius);

	buildingMesh->ComponentTags.Add(FName("Building"));

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/Game_Assets/Blueprints/BarracksBlowingUp.BarracksBlowingUp'"));
	if (ItemBlueprint.Object) {
		ExplosionBlueprint = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void ABuilding_TechCenter::BeginPlay()
{
	Super::BeginPlay();
	
	buildingMesh->SetWorldScale3D(FVector(6));
}

void ABuilding_TechCenter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (setPlayerOwner != nullptr) {
		InitializeStructure(Cast<II_Player>(setPlayerOwner), "Placeholder", 10.0f);
		SetName(TEXT("TechCenter"));
		SetMaxHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("TechCenter")), (TEXT("Context")), false)->MaxHealth);
		SetCurrentHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("TechCenter")), (TEXT("Context")), false)->MaxHealth);
		setPlayerOwner = nullptr;
		return;
	}

}