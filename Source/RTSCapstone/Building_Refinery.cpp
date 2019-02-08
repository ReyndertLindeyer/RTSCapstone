// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Refinery.h"

ABuilding_Refinery::ABuilding_Refinery() {
	PrimaryActorTick.bCanEverTick = true;
	//Setting up general values
	team = 1;
	maxHealth = 500;
	currentHealth = maxHealth;
	powerUsage = 20;
	spawnTime = 2;
	cost = 100;
	buildRadius = 500;
	isBuilding = true;
	canSpawnHarvester = true;

	this->Tags.Add(FName("Refinery"));

	buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RefineryMesh"));
	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Refinery.Placeholder_Refinery")).Get());
	buildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	RootComponent = buildingMesh;
	buildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	buildingMesh->SetCollisionProfileName(TEXT("Trigger"));
	buildingMesh->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_Refinery::BeginOverlap);
	buildingMesh->OnComponentEndOverlap.AddDynamic(this, &ABuilding_Refinery::OnOverlapEnd);
	buildingMesh->SetSimulatePhysics(false);

	decal->CreateDynamicMaterialInstance();
	decal->RelativeRotation = FRotator(-90, 0, 0);
	decal->AttachTo(RootComponent);
	decal->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	decal->DecalSize = FVector(2, buildRadius, buildRadius);

	buildRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("buildRadiusSphere"));
	buildRadiusSphere->InitSphereRadius(buildRadius);
	buildRadiusSphere->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_Refinery::BeginOverlap);
	buildRadiusSphere->OnComponentEndOverlap.AddDynamic(this, &ABuilding_Refinery::OnOverlapEnd);
	buildRadiusSphere->AttachTo(RootComponent);

	buildingMesh->ComponentTags.Add(FName("Building"));
	buildRadiusSphere->ComponentTags.Add(FName("buildRadius"));
	decal->ComponentTags.Add(FName("BuildArea"));
}

void ABuilding_Refinery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isPlaced) {
		buildingMesh->SetWorldLocation(FMath::VInterpTo(buildingMesh->GetComponentLocation(), FVector(buildingMesh->GetComponentLocation().X, buildingMesh->GetComponentLocation().Y, 33.0f), DeltaTime, spawnTime));
		if (buildingMesh->GetComponentLocation().Z >= 33.0f) {
			PrimaryActorTick.bCanEverTick = false;
			if (canSpawnHarvester) {
				//Spawn a harvester
				//Harvester will know when it reaches the refinery and will unload its resources at that time

				AUnit_Harvester_Character* temp = GetWorld()->SpawnActor<AUnit_Harvester_Character>(AUnit_Harvester_Character::StaticClass(), FVector(this->GetActorLocation() + FVector(30.0f, 10.0f, 0.0f)), FRotator(0.0f, 0.0f, 0.0f));
				temp->SetActorScale3D(FVector(0.5, 0.5, 0.5));

				canSpawnHarvester = false;
			}
		}
	}
}