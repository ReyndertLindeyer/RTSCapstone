// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_TechCenter.h"

ABuilding_TechCenter::ABuilding_TechCenter() {
	PrimaryActorTick.bCanEverTick = true;
	//Setting up general values
	team = 1;
	spawnTime = 2;
	buildRadius = 500;
	isBuilding = true;

	SetHitRadius(200);

	buildingMesh->SetWorldScale3D(FVector(6));

	static ConstructorHelpers::FObjectFinder<USoundCue> select(TEXT("/Game/Game_Assets/Sounds/Building_Sounds_V1/Tech_Centre_-_Select_Cue"));
	selectCue = select.Object;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devTechCentre.devTechCentre")).Get());
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(3, buildRadius, buildRadius);

	buildingMesh->ComponentTags.Add(FName("Building"));

	static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("Class'/Game/Game_Assets/Blueprints/BarracksBlowingUp.BarracksBlowingUp_C'"));
	if (ItemBlueprint.Object) {
		ExplosionBlueprint = (UClass*)ItemBlueprint.Object;
	}
}

// Called when the game starts or when spawned
void ABuilding_TechCenter::BeginPlay()
{
	Super::BeginPlay();
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

bool ABuilding_TechCenter::constructAtLocation(II_Player* player)
{
	dustParticleComp->SetWorldLocation(this->GetActorLocation());
	dustParticleComp->ActivateSystem();
	tempHeight = RootComponent->GetComponentLocation().Z;
	buildingMesh->SetWorldLocation(FVector(RootComponent->GetComponentLocation().X, RootComponent->GetComponentLocation().Y, RootComponent->GetComponentLocation().Z - 725));


	return false;
}