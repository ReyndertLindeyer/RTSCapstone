// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Construction_Yard.h"


ABuilding_Construction_Yard::ABuilding_Construction_Yard() {
	PrimaryActorTick.bCanEverTick = false;
	team = 1;
	maxHealth = 2000;
	currentHealth = maxHealth;
	powerUsage = 0;
	spawnTime = 0;
	cost = 100;
	isBuilding = false;

	buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConstructionYardMesh"));
	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Construction_Yard.Placeholder_Construction_Yard")).Get());
	buildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	RootComponent = buildingMesh;
	buildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	buildingMesh->SetCollisionProfileName(TEXT("Trigger"));
	buildingMesh->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_Construction_Yard::BeginOverlap);
	buildingMesh->OnComponentEndOverlap.AddDynamic(this, &ABuilding_Construction_Yard::OnOverlapEnd);
	buildingMesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void ABuilding_Construction_Yard::BeginPlay()
{
	Super::BeginPlay();
	buildingMesh->SetMaterial(0, canBuildIndicator);
}

// Called every frame
void ABuilding_Construction_Yard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABuilding_Construction_Yard::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		buildingMesh->SetMaterial(0, cantBuildIndicator);
	}
}

void ABuilding_Construction_Yard::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		buildingMesh->SetMaterial(0, canBuildIndicator);
	}
}