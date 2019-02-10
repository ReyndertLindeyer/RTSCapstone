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

	canBuildIndicator = CreateDefaultSubobject<UMaterial>(TEXT("GreenBuildingGhost"));
	canBuildIndicator = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/Game_Assets/Materials/GreenBuildingGhost")).Get();

	cantBuildIndicator = CreateDefaultSubobject<UMaterial>(TEXT("cantBuildIndicator"));
	cantBuildIndicator = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/Game_Assets/Materials/RedBuildingGhost")).Get();

	regularMaterial = CreateDefaultSubobject<UMaterial>(TEXT("regularMaterial"));
	regularMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/Game_Assets/Materials/regularMaterial")).Get();

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
}

void ABuildingMaster::DisableBuildDecal()
{
	decal->SetVisibility(false);
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
}

void ABuildingMaster::DeselectBuilding()
{
	selectedDecal->SetVisibility(false);
}

// Called when the game starts or when spawned
void ABuildingMaster::BeginPlay()
{
	Super::BeginPlay();
	buildingMesh->SetMaterial(0, cantBuildIndicator);
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

bool ABuildingMaster::constructAtLocation()
{
	if (!overlapping && isInRadius) {
		buildingMesh->SetMaterial(0, regularMaterial);
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
				buildingMesh->SetMaterial(0, cantBuildIndicator);
			}
			numOfBuildingCollisions++;
			///Code to check to see if the building is within range of another
			/*
			if ("CapsuleComp" == OverlappedComponent->GetName) {
				buildingMesh->SetMaterial(0, canBuildIndicator);
			}
			*/
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
				buildingMesh->SetMaterial(0, canBuildIndicator);
				numOfBuildingCollisions = 0;
			}
		}
	}
}

void ABuildingMaster::BeginRadiusOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		if (!constructed && OverlappedComponent->ComponentHasTag(FName("buildRadius")) && numOfBuildingCollisions <= 0) {
			buildingMesh->SetMaterial(0, canBuildIndicator);
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
				buildingMesh->SetMaterial(0, cantBuildIndicator);
				numOfRadiusCollisions = 0;
			}
		}
	}
}