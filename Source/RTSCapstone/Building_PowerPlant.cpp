// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_PowerPlant.h"

ABuilding_PowerPlant::ABuilding_PowerPlant() {
	PrimaryActorTick.bCanEverTick = true;
	//Setting up general values
	team = 1;
	spawnTime = 2;
	buildRadius = 5000;
	isBuilding = true;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/PowerPlant_Model/PowerPlant.PowerPlant")).Get());
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(10, buildRadius, buildRadius);

	static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("Class'/Game/Game_Assets/Blueprints/BarracksBlowingUp.BarracksBlowingUp_C'"));
	if (ItemBlueprint.Object) {
		ExplosionBlueprint = (UClass*)ItemBlueprint.Object;
	}

}

void ABuilding_PowerPlant::BeginPlay()
{
	Super::BeginPlay();

}

void ABuilding_PowerPlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (setPlayerOwner != nullptr) {
		InitializeStructure(Cast<II_Player>(setPlayerOwner), "Placeholder", 10.0f);
		SetName(TEXT("Powerplant"));
		SetMaxHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("Powerplant")), (TEXT("Context")), false)->MaxHealth);
		SetCurrentHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("Powerplant")), (TEXT("Context")), false)->MaxHealth);
		setPlayerOwner = nullptr;
		return;
	}
}


void ABuilding_PowerPlant::Upgrade()
{
	///Set new mesh
	/*
	buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerPlantMesh"));
	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Power_Plant.Placeholder_Power_Plant")).Get());
	buildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	RootComponent = buildingMesh;
	buildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	buildingMesh->SetSimulatePhysics(false);
	*/
}

bool ABuilding_PowerPlant::constructAtLocation(II_Player* player)
{
	dustParticleComp->SetWorldLocation(this->GetActorLocation());
	dustParticleComp->ActivateSystem();
	tempHeight = RootComponent->GetComponentLocation().Z;
	buildingMesh->SetWorldLocation(FVector(RootComponent->GetComponentLocation().X, RootComponent->GetComponentLocation().Y, RootComponent->GetComponentLocation().Z - 550));


	return false;
}