// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceSpawner.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AResourceSpawner::AResourceSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	spawnRadius = 2000.0f;
	richSpawnRadius = 200.0f;
	richness = 200;

	root = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	root->InitCapsuleSize(spawnRadius, 20);
	root->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = root;
}

// Called when the game starts or when spawned
void AResourceSpawner::BeginPlay()
{
	Super::BeginPlay();

	float distance = spawnRadius / richness;
	//Will use the location of the spawner as a base to spawn resources
	for (int i = 0; i <= richness; i++) {
		for (int j = 0; j <= richness; j++) {
			//Creates the spawn location for actors
			FVector spawnLocation = RootComponent->GetComponentLocation() + FVector(i * richness - spawnRadius, j * richness - spawnRadius, 2.0f);

			//Checks to see if they are within the determined spawn radius
			float distance = FVector::Dist(RootComponent->GetComponentLocation(), spawnLocation);

			//Small randomization of variables to remove grid
			spawnLocation = FVector(spawnLocation.X + FMath::RandRange(-100.0f, 100.0f), spawnLocation.Y + FMath::RandRange(-100.0f, 100.0f), spawnLocation.Z);

			if (distance < spawnRadius && distance < richSpawnRadius && distance > 1.0f) {
				//Spawn rich resources
				AResourceNode* temp = GetWorld()->SpawnActor<AResourceNode>(AResourceNode::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
				temp->setup(FMath::RandRange(200, 220), true);
			}
			else if (distance < spawnRadius && distance > 1.0f) {
				//Spawn regular resources
				AResourceNode* temp = GetWorld()->SpawnActor<AResourceNode>(AResourceNode::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
				temp->setup(FMath::RandRange(100, 120), false);
			}
		}
	}
}

// Called every frame
void AResourceSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

