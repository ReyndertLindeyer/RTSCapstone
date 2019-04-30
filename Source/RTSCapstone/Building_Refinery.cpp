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

	SetHitRadius(400);

	static ConstructorHelpers::FObjectFinder<USoundCue> select(TEXT("/Game/Game_Assets/Sounds/Building_Sounds_V1/Refinery_-_Select_Cue"));
	selectCue = select.Object;

	this->Tags.Add(FName("Refinery"));
	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Refinery_Model/Resource_Refinery.Resource_Refinery")).Get());
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(100, buildRadius, buildRadius);

	buildingMesh->ComponentTags.Add(FName("Building"));


	harvestPt = CreateDefaultSubobject<USceneComponent>(TEXT("Harvest Point"));
	harvestPt->SetupAttachment(RootComponent);
	harvestPt->SetRelativeLocation(FVector(0.0f, 400.0f, 40.0f));

	isOccupied = false;

	static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("Class'/Game/Game_Assets/Blueprints/Props/BlowingUpRefinery.BlowingUpRefinery_C'"));
	if (ItemBlueprint.Object) {
		ExplosionBlueprint = (UClass*)ItemBlueprint.Object;
	}
	
}

void ABuilding_Refinery::BeginPlay()
{
	Super::BeginPlay();


	selectedDecal->DecalSize = FVector(200, 220, 220);
	
}

void ABuilding_Refinery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (setPlayerOwner != nullptr) {
		InitializeStructure(Cast<II_Player>(setPlayerOwner), "Placeholder", 10.0f);
		SetName(TEXT("Refinery"));
		SetMaxHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("Refinery")), (TEXT("Context")), false)->MaxHealth);
		SetCurrentHealth(GetEntityOwner()->GetBuildingDataTable()->FindRow<FBuildingVariables>(FName(TEXT("Refinery")), (TEXT("Context")), false)->MaxHealth);
		setPlayerOwner = nullptr;
		return;
	}

	
	
	if (constructed && canSpawnHarvester)
	{	
		InitializeRefinery();
		canSpawnHarvester = false;
	}
}

void ABuilding_Refinery::InitializeRefinery()
{
	//Spawn the harvester
	FActorSpawnParameters SpawnInfo;
	AActor* spawnedHarvy = GetWorld()->SpawnActor<AUNIT_Harvester>(AUNIT_Harvester::StaticClass(), FVector(harvestPt->GetComponentLocation().X, harvestPt->GetComponentLocation().Y, harvestPt->GetComponentLocation().Z + 100), FRotator(0.0f, 0.0f, 0.0f));
	Cast<II_Entity>(spawnedHarvy)->InitializeEntity(GetEntityOwner(), "Harvester", 2000.0f);
}

bool ABuilding_Refinery::constructAtLocation(II_Player* player)
{
	dustParticleComp->SetWorldLocation(this->GetActorLocation());
	dustParticleComp->ActivateSystem();
	tempHeight = RootComponent->GetComponentLocation().Z;
	buildingMesh->SetWorldLocation(FVector(RootComponent->GetComponentLocation().X, RootComponent->GetComponentLocation().Y, RootComponent->GetComponentLocation().Z - 825));


	return false;
}