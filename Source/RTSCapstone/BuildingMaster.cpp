// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingMaster.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABuildingMaster::ABuildingMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	RootComponent = buildingMesh;
	buildingMesh->SetWorldScale3D(FVector(2, 2, 2));
	//buildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	buildingMesh->SetCollisionProfileName(TEXT("OverlapAll"));

	//Create the building area decal and sets the material, has to rotate by -90 for some reason
	decal = CreateDefaultSubobject<UDecalComponent>(TEXT("buildAreaDecal"));
	decal->SetDecalMaterial(ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("/Game/Game_Assets/Materials/MAT_Decal_RoundBuildingRadius")).Get());
	decal->CreateDynamicMaterialInstance();
	decal->RelativeRotation = FRotator(-90, 0, 0);
	decal->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));


	selectedDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("selectionDecal"));
	selectedDecal->SetDecalMaterial(ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("/Game/Game_Assets/Materials/MAT_Decal_SelectionCircle")).Get());
	selectedDecal->CreateDynamicMaterialInstance();
	selectedDecal->RelativeRotation = FRotator(-90, 0, 0);
	selectedDecal->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	selectedDecal->SetupAttachment(RootComponent);

	decal->SetVisibility(false);
	selectedDecal->SetVisibility(false);

	constructed = false;
	overlapping = false;
	isPlaced = false;
	isInRadius = false;
	buildRadiusActive = false;
	team = 1;
	spawnTime = 1.0f;
	buildRadius = 0.0f;
	sightRadius = 300;
	numOfBuildingCollisions = 0;

	buildingMesh->ComponentTags.Add(FName("Building"));
	decal->ComponentTags.Add(FName("BuildArea"));
}

void ABuildingMaster::EnableBuildDecal()
{
	decal->SetVisibility(true);
	PrimaryActorTick.bCanEverTick = true;
	buildRadiusActive = true;
}

void ABuildingMaster::DisableBuildDecal()
{
	decal->SetVisibility(false); 
	PrimaryActorTick.bCanEverTick = false;
	buildRadiusActive = false;
}

int32 ABuildingMaster::GetHeight()
{
	return buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.Z;
}

float ABuildingMaster::GetSightRadius()
{
	return sightRadius;
}

void ABuildingMaster::SetSelection(bool selectionType)
{
	selectedDecal->SetVisibility(selectionType);
	selected = selectionType;
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
	isInRadius = false;

	if (isPlaced) {
		buildingMesh->SetWorldLocation(FMath::VInterpTo(buildingMesh->GetComponentLocation(), FVector(buildingMesh->GetComponentLocation().X, buildingMesh->GetComponentLocation().Y, buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.Z + 20), DeltaTime, spawnTime));
		if (buildingMesh->GetComponentLocation().Z >= buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.Z + 20) {
			PrimaryActorTick.bCanEverTick = false;
		}
	}

	if (buildRadiusActive) {
		// Detect all AActors within a Radius
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		TArray<AActor*> ignoreActors;
		TArray<AActor*> outActors;

		ignoreActors.Add(this);

		DrawDebugSphere(GetWorld(), GetActorLocation(), buildRadius * 2, 24, FColor(0, 0, 255));

		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), buildRadius * 2, objectTypes, nullptr, ignoreActors, outActors);

		for (int i = 0; i < outActors.Num(); i++) {
			if (Cast<ABuildingMaster>(outActors[i])) {
				if(!Cast<ABuildingMaster>(outActors[i])->GetIsInRadius())
					Cast<ABuildingMaster>(outActors[i])->isInRadius = true;
			}
		}
	}
}

UStaticMeshComponent * ABuildingMaster::GetBuildingMesh()
{
	return buildingMesh;
}

bool ABuildingMaster::constructAtLocation(II_Player* player)
{
	if (!overlapping && isInRadius) {
		buildingMesh->SetWorldLocation(FVector(RootComponent->GetComponentLocation().X, RootComponent->GetComponentLocation().Y, RootComponent->GetComponentLocation().Z - buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.Z));
		constructed = true;
		isPlaced = true;

		//buildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		buildingMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

		return true;
	}
	return false;
}

void ABuildingMaster::InitializeStructure(II_Player* player_)
{
	// Do Nothing
}



void ABuildingMaster::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		if (!constructed)
		{
			if (!overlapping) {
				overlapping = true;
				UE_LOG(LogTemp, Warning, TEXT("Overlapping"));
			}
			numOfBuildingCollisions++;
		}
	}
}

void ABuildingMaster::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		if (!constructed)
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

void ABuildingMaster::DestroyEntity() 
{
	//Destroy(this);
}