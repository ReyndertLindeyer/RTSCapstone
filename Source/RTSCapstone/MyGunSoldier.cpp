// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGunSoldier.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h" 

// Sets default values
AMyGunSoldier::AMyGunSoldier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	root = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	RootComponent = root;
	root->InitCapsuleSize(6.0f, 15.0f);
	root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	root->SetSimulatePhysics(true);

	//static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/Placeholder_Soldier_Gun_Cylinder001.fbx"));
	//UStaticMesh* Asset = MeshAsset.Object;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SoldierMesh"));
	mesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Soldier_Gun_Cylinder001.Placeholder_Soldier_Gun_Cylinder001")).Get());
	mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -8.0f));
	mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMyGunSoldier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyGunSoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

