// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceSpawner.h"

// Sets default values
AResourceSpawner::AResourceSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResourceSpawnerMesh"));
	mesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Soldier_Gun_Cylinder001.Placeholder_Soldier_Gun_Cylinder001")).Get());
	mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	mesh->SetupAttachment(RootComponent);

	spawnRadius = 100.0f;
	richSpawnRadius = 20.0f;
}

// Called when the game starts or when spawned
void AResourceSpawner::BeginPlay()
{
	Super::BeginPlay();

	int num = 20;

	//Will use the location of the spawner as a base to spawn resources
	for (int i = 0; i > num; i++) {
		FVector spawnLocation = RootComponent->GetComponentLocation() + FVector(i / 2 * 20 + spawnRadius / 2, i % 2 * 20 + spawnRadius / 2, 0);
		float distance = FVector::Dist(RootComponent->GetComponentLocation(), spawnLocation);
		if (distance < spawnRadius && distance < richSpawnRadius) {
			//Spawn rich resources
			AResourceNode* temp = GetWorld()->SpawnActor<AResourceNode>(GetClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
			temp->yield = FMath::RandRange(200, 220);
			richResources.Add(temp);
		}
		else if (distance < spawnRadius) {
			//Spawn regular resources
			AResourceNode* temp = GetWorld()->SpawnActor<AResourceNode>(GetClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
			temp->yield = FMath::RandRange(100, 120);
			regularResources.Add(temp);
		}
	}
}

// Called every frame
void AResourceSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

