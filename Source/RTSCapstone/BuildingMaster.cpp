// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingMaster.h"

// Sets default values
ABuildingMaster::ABuildingMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	buildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	RootComponent = buildingMesh;
	buildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	buildingMesh->SetCollisionProfileName(TEXT("Trigger"));

	//Create the building area decal and sets the material, has to rotate by -90 for some reason
	decal = CreateDefaultSubobject<UDecalComponent>(TEXT("buildAreaDecal"));
	decal->SetDecalMaterial(ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/Game_Assets/Materials/MAT_Decal_RoundBuildingRadius")).Get());
	decal->CreateDynamicMaterialInstance();
	decal->RelativeRotation = FRotator(-90, 0, 0);
	decal->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));


	selectedDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("selectionDecal"));
	selectedDecal->SetDecalMaterial(ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/Game_Assets/Materials/MAT_Decal_SelectionCircle")).Get());
	selectedDecal->CreateDynamicMaterialInstance();
	selectedDecal->RelativeRotation = FRotator(-90, 0, 0);
	selectedDecal->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	selectedDecal->SetupAttachment(RootComponent);

	decal->SetVisibility(false);
	selectedDecal->SetVisibility(false);

	buildRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("buildRadiusSphere"));

	constructed = false;
	overlapping = false;
	isPlaced = false;
	isInRadius = false;
	maxHealth = 10;
	currentHealth = 10;
	powerUsage = 10;
	team = 1;
	spawnTime = 1.0f;
	sightRadius = 300;
	numOfBuildingCollisions = 0;
	numOfRadiusCollisions = 0;
}

uint32 ABuildingMaster::GetPowerUsage()
{
	return powerUsage;
}

uint32 ABuildingMaster::GetCost()
{
	return cost;
}

void ABuildingMaster::EnableBuildDecal()
{
	decal->SetVisibility(true);
	buildRadiusSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABuildingMaster::DisableBuildDecal()
{
	decal->SetVisibility(false);
	buildRadiusSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

int32 ABuildingMaster::GetHeight()
{
	return buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.Z;
}

float ABuildingMaster::GetSightRadius()
{
	return sightRadius;
}

void ABuildingMaster::SelectBuilding()
{
	selectedDecal->SetVisibility(true);
	selected = true;
}

void ABuildingMaster::DeselectBuilding()
{
	selectedDecal->SetVisibility(false);
	selected = false;
}

bool ABuildingMaster::GetIsOverlapping()
{
	return overlapping;
}

bool ABuildingMaster::GetIsInRadius()
{
	return isInRadius;
}

bool ABuildingMaster::IsSelected()
{
	return selected;
}

bool ABuildingMaster::IsDead()
{
	if(currentHealth == 0){
		return true;
	}
	return false;
}

void ABuildingMaster::Suicide()
{
	//Destroy self and play particle effect
	Destroy();
	//In future replace this with destructing mesh that lowers into ground before deletion
}

// Called when the game starts or when spawned
void ABuildingMaster::BeginPlay()
{
	Super::BeginPlay();
	if(buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.Y > buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.X)
		selectedDecal->DecalSize = FVector(2, buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.Y + 20, buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.Y + 20);
	else
		selectedDecal->DecalSize = FVector(2, buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.X + 20, buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.X + 20);


}

// Called every frame
void ABuildingMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isPlaced) {
		buildingMesh->SetWorldLocation(FMath::VInterpTo(buildingMesh->GetComponentLocation(), FVector(buildingMesh->GetComponentLocation().X, buildingMesh->GetComponentLocation().Y, 33.0f), DeltaTime, spawnTime));
		if (buildingMesh->GetComponentLocation().Z >= 33.0f) {
			PrimaryActorTick.bCanEverTick = false;
		}
	}
}

UStaticMeshComponent * ABuildingMaster::GetBuildingMesh()
{
	return buildingMesh;
}

bool ABuildingMaster::constructAtLocation()
{
	if (!overlapping && isInRadius) {
		buildingMesh->SetWorldLocation(FVector(RootComponent->GetComponentLocation().X, RootComponent->GetComponentLocation().Y, RootComponent->GetComponentLocation().Z - 40.0f));
		constructed = true;
		isPlaced = true;

		buildRadiusSphere->SetSphereRadius(buildRadius);

		return true;
	}
	return false;
}

int ABuildingMaster::GetHealth()
{
	return (int)currentHealth;
}

int ABuildingMaster::GetMaxHealth()
{
	return (int)maxHealth;
}



void ABuildingMaster::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		if (!constructed && OtherComp->ComponentHasTag(FName("Building")))
		{
			if (!overlapping) {
				overlapping = true;
			}
			numOfBuildingCollisions++;
		}
	}
}

void ABuildingMaster::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		if (!constructed && OtherComp->ComponentHasTag(FName("Building")))
		{
			if (numOfBuildingCollisions > 0) {
				numOfBuildingCollisions--;
			}
			if (numOfBuildingCollisions <= 0)
			{
				overlapping = false;
				numOfBuildingCollisions = 0;
			}
		}
	}
}

void ABuildingMaster::BeginRadiusOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		if (!constructed && OverlappedComponent->ComponentHasTag(FName("buildRadius")) && numOfBuildingCollisions <= 0) {
			isInRadius = true;
			numOfRadiusCollisions++;
		}
	}
}

void ABuildingMaster::OnRadiusOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		if (!constructed && OverlappedComp->ComponentHasTag(FName("buildRadius")) && numOfBuildingCollisions <= 0)
		{
			if (numOfRadiusCollisions > 0) {
				numOfRadiusCollisions--;
			}
			if (numOfRadiusCollisions <= 0)
			{
				isInRadius = false;
				numOfRadiusCollisions = 0;
			}
		}
	}
}