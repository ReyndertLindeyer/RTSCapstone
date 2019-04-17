// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Outpost.h"


ABuilding_Outpost::ABuilding_Outpost() {
	PrimaryActorTick.bCanEverTick = false;
	//Setting up general values
	spawnTime = 2;
	buildRadius = 1000;
	isBuilding = true;

	SetHitRadius(40);

	static ConstructorHelpers::FObjectFinder<USoundCue> select(TEXT("/Game/Game_Assets/Sounds/Building_Sounds_V1/Outpost_-_Select_Cue"));
	selectCue = select.Object;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Construction_Yard.Placeholder_Construction_Yard")).Get());
	RootComponent = buildingMesh;
	buildingMesh->SetSimulatePhysics(false);
	buildingMesh->bReceivesDecals = false;

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(100, buildRadius / 2.5f, buildRadius / 2.5f);

	static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("Class'/Game/Game_Assets/Blueprints/Props/BlowingUpVehicleFactory.BlowingUpVehicleFactory_C'"));
	if (ItemBlueprint.Object) {
		ExplosionBlueprint = (UClass*)ItemBlueprint.Object;
	}
}


void ABuilding_Outpost::BeginPlay()
{
	Super::BeginPlay();
	decal->SetRelativeLocation(FVector(0.0));
	selectedDecal->DecalSize = FVector(200, 60, 60);

}