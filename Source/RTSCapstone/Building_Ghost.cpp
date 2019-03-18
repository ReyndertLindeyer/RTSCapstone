// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Ghost.h"

// Sets default values
ABuilding_Ghost::ABuilding_Ghost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	buildingMesh->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_Ghost::BeginOverlap);
	buildingMesh->OnComponentEndOverlap.AddDynamic(this, &ABuilding_Ghost::OnOverlapEnd);
	RootComponent = buildingMesh;
	buildingMesh->SetWorldScale3D(FVector(2, 2, 2));
	buildingMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	radius = 20000;

	numOfBuildingCollisions = 0;

	overlapping = false;

	buildingMesh->SetCanEverAffectNavigation(false);
}

// Called when the game starts or when spawned
void ABuilding_Ghost::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuilding_Ghost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Detect all AActors within a Radius
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TArray<AActor*> ignoreActors;
	TArray<AActor*> outActors;

	ignoreActors.Add(this);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), radius * 2, objectTypes, nullptr, ignoreActors, outActors);

	for (int i = 0; i < outActors.Num(); i++) {
		if (Cast<ABuildingMaster>(outActors[i])) {
			if (FVector::Dist(GetActorLocation(), outActors[i]->GetActorLocation()) < Cast<ABuildingMaster>(outActors[i])->GetConstructionRadius() * 2) {
				isInRadius = true;
			}
			else {
				isInRadius = false;
			}
		}
	}
}

bool ABuilding_Ghost::GetIsOverlapping()
{
	return overlapping;
}

bool ABuilding_Ghost::GetIsInRadius()
{
	return isInRadius;
}


UStaticMeshComponent * ABuilding_Ghost::GetBuildingMesh()
{
	return buildingMesh;
}


void ABuilding_Ghost::SetMesh(UStaticMesh* inMesh)
{
	buildingMesh->SetStaticMesh(inMesh);
}


void ABuilding_Ghost::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
			if (!overlapping) {
				overlapping = true;
			}
			numOfBuildingCollisions++;
	}
}

void ABuilding_Ghost::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
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