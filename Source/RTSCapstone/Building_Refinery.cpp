// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Refinery.h"
#include "UNIT_Harvester.h"

ABuilding_Refinery::ABuilding_Refinery() {
	PrimaryActorTick.bCanEverTick = true;
	//Setting up general values
	team = 1;
	spawnTime = 2;
	buildRadius = 500;
	isBuilding = true;
	canSpawnHarvester = true;

	this->Tags.Add(FName("Refinery"));
	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/devRefinery_v1.devRefinery_v1")).Get());
	buildingMesh->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_Refinery::BeginOverlap);
	buildingMesh->OnComponentEndOverlap.AddDynamic(this, &ABuilding_Refinery::OnOverlapEnd);
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(2, buildRadius, buildRadius);

	buildingMesh->ComponentTags.Add(FName("Building"));
	decal->ComponentTags.Add(FName("BuildArea"));


	harvestPt = CreateDefaultSubobject<USceneComponent>(TEXT("Harvest Point"));
	harvestPt->SetupAttachment(RootComponent);
	harvestPt->SetRelativeLocation(FVector(-75.0f, 0.0f, 40.0f));

	isOccupied = false;
	
}

void ABuilding_Refinery::BeginPlay()
{
	Super::BeginPlay();

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

				//AUnit_Harvester_Character* temp = GetWorld()->SpawnActor<AUnit_Harvester_Character>(AUnit_Harvester_Character::StaticClass(), FVector(this->GetActorLocation() + FVector(30.0f, 10.0f, 0.0f)), FRotator(0.0f, 0.0f, 0.0f));
				//temp->SetActorScale3D(FVector(0.5, 0.5, 0.5));

				canSpawnHarvester = false;
			}
		}
	}
}

void ABuilding_Refinery::InitializeStructure(II_Player* player)
{
	InitializeEntity(player, "Refinery", 2000.0f);

	FActorSpawnParameters SpawnInfo;
	//AActor* MyActor = GWorld->SpawnActor<AUNIT_Harvester>(AUNIT_Harvester::StaticClass(), harvestPt->GetRelativeTransform(), FRotator::ZeroRotator, SpawnInfo);
	AActor* spawnedHarvy = GetWorld()->SpawnActor<AUNIT_Harvester>(AUNIT_Harvester::StaticClass(), harvestPt->GetComponentLocation(), FRotator(0.0f, 0.0f, 0.0f));
	Cast<II_Entity>(spawnedHarvy)->InitializeEntity(GetEntityOwner(), "Harvester", 2000.0f);

}