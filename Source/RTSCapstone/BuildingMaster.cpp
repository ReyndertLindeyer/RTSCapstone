// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingMaster.h"

// Sets default values
ABuildingMaster::ABuildingMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	canBuildIndicator = CreateDefaultSubobject<UMaterial>(TEXT("GreenBuildingGhost"));
	canBuildIndicator = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/Game_Assets/Materials/GreenBuildingGhost")).Get();

	cantBuildIndicator = CreateDefaultSubobject<UMaterial>(TEXT("cantBuildIndicator"));
	cantBuildIndicator = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/Game_Assets/Materials/RedBuildingGhost")).Get();

	regularMaterial = CreateDefaultSubobject<UMaterial>(TEXT("regularMaterial"));
	regularMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/Game_Assets/Materials/regularMaterial")).Get();

	constructed = false;
	overlapping = false;
	isPlaced = false;
}

uint32 ABuildingMaster::GetPowerUsage()
{
	return powerUsage;
}

uint32 ABuildingMaster::GetCost()
{
	return cost;
}

// Called when the game starts or when spawned
void ABuildingMaster::BeginPlay()
{
	Super::BeginPlay();
	buildingMesh->SetMaterial(0, canBuildIndicator);
	
}

// Called every frame
void ABuildingMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!constructed && isPlaced) {
		buildingMesh->SetWorldLocation(FMath::VInterpTo(buildingMesh->GetComponentLocation(), FVector(buildingMesh->GetComponentLocation().X, buildingMesh->GetComponentLocation().Y, 40.0f), DeltaTime, 1));
		if (buildingMesh->GetComponentLocation().Z >= 36.0f) {
			constructed = true;
		}
	}
}

void ABuildingMaster::constructAtLocation()
{
	buildingMesh->SetMaterial(0, regularMaterial);
	buildingMesh->SetWorldLocation(FVector(RootComponent->GetComponentLocation().X, RootComponent->GetComponentLocation().Y, RootComponent->GetComponentLocation().Z - 40.0f));
	constructed = false;
	isPlaced = true;
}



void ABuildingMaster::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	if (OtherActor && (OtherActor != this) && OtherComp && !constructed)
	{
		overlapping = true;
		buildingMesh->SetMaterial(0, cantBuildIndicator);
	}
}

void ABuildingMaster::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp && !constructed)
	{
		overlapping = false;
		buildingMesh->SetMaterial(0, canBuildIndicator);
	}
}