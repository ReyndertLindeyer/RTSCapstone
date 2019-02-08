// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Construction_Yard.h"


ABuilding_Construction_Yard::ABuilding_Construction_Yard() {
	PrimaryActorTick.bCanEverTick = false;
	team = 1;
	maxHealth = 2000;
	currentHealth = maxHealth;
	powerUsage = 0;
	spawnTime = 0;
	buildRadius = 1000;
	cost = 100;
	isBuilding = false;

	buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConstructionYardMesh"));
	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Construction_Yard.Placeholder_Construction_Yard")).Get());
	buildingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	RootComponent = buildingMesh;
	buildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	buildingMesh->SetCollisionProfileName(TEXT("Trigger"));
	buildingMesh->SetSimulatePhysics(false);

	decal->CreateDynamicMaterialInstance();
	decal->RelativeRotation = FRotator(-90, 0, 0);
	decal->AttachTo(RootComponent);
	decal->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	decal->DecalSize = FVector(2, buildRadius, buildRadius);

	buildRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("buildRadiusSphere"));
	buildRadiusSphere->InitSphereRadius(buildRadius);
	buildRadiusSphere->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_Construction_Yard::BeginOverlap);
	buildRadiusSphere->OnComponentEndOverlap.AddDynamic(this, &ABuilding_Construction_Yard::OnOverlapEnd);
	buildRadiusSphere->AttachTo(RootComponent);

	buildingMesh->ComponentTags.Add(FName("Building"));
	buildRadiusSphere->ComponentTags.Add(FName("buildRadius"));
	decal->ComponentTags.Add(FName("BuildArea"));
}

// Called when the game starts or when spawned
void ABuilding_Construction_Yard::BeginPlay()
{
	Super::BeginPlay();
	buildingMesh->SetMaterial(0, regularMaterial);
	constructed = true;
	isPlaced = true;
}

// Called every frame
void ABuilding_Construction_Yard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}