// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_BaseManager.h"
#include "DrawDebugHelpers.h"

// Sets default values
AEnemy_BaseManager::AEnemy_BaseManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	isStartingArea = true;
	PrimaryActorTick.bCanEverTick = isStartingArea;

	baseSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	baseSphere->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere.Sphere")).Get());

	baseMeshes.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Power_Plant.Placeholder_Power_Plant")).Get());
	baseMeshes.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Refinery.Placeholder_Refinery")).Get());
	baseMeshes.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Barracks.Placeholder_Barracks")).Get());
	baseMeshes.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Power_Plant.Placeholder_Power_Plant")).Get());
	baseMeshes.Add(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Power_Plant.Placeholder_Power_Plant")).Get());

	//baseRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("baseRadius"));
	//baseRadiusSphere->SetSphereRadius(40);
	RootComponent = baseSphere;
	baseSphere->SetWorldScale3D(FVector(8));

}

// Called when the game starts or when spawned
void AEnemy_BaseManager::BeginPlay()
{
	Super::BeginPlay();

	if (buildingsArray.Num() > 0) {
		for (int32 i = 0; i < buildingsArray.Num(); i++) {
			UE_LOG(LogTemp, Warning, TEXT("%d"), buildingsArray.Num());
			buildingsArray[i]->SetMesh(baseMeshes[buildingsArray[i]->buildingType - 1]);
			buildingsArray[i]->myManager = this;
			if(!isStartingArea)
				counterArray.Add(FMath::RandRange(10.0f, 15.0f));
			else
				counterArray.Add(FMath::RandRange(60.0f, 70.0f));

			if (launchPoint) {
				buildingsArray[i]->SetupBulding(launchPoint->GetActorLocation());
			}
		}
	}

	PrimaryActorTick.bCanEverTick = isStartingArea;
	if (launchPoint && isStartingArea) {
		launchPoint->EnableTick();
	}
}

// Called every frame
void AEnemy_BaseManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isStartingArea) {

		for (int32 i = 0; i < buildingsArray.Num(); i++) {
			if (buildingsArray[i] != nullptr) {
				counterArray[i] -= DeltaTime;
				if (counterArray[i] <= 0.0f) {
					buildingsArray[i]->SpawnUnit();
					counterArray[i] = FMath::RandRange(10.0f, 15.0f);
				}
			}
		}
		if (buildingsArray.Num() == 0) {
			GetWorld()->SpawnActor<AResourceSpawner>(AResourceSpawner::StaticClass(), this->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f));
			for (int32 i = 0; i < adjacentManagerArray.Num(); i++) {
				if (adjacentManagerArray[i] != nullptr)
					adjacentManagerArray[i]->ActivateManager();
			}
			Destroy(this);
		}
	}
}

void AEnemy_BaseManager::ActivateManager() {
	PrimaryActorTick.bCanEverTick = true;
	isStartingArea = true;
	if (launchPoint) {
		launchPoint->EnableTick();
	}
}

void AEnemy_BaseManager::RemoveBuilding(ABuilding_Enemy_Spawner* inBuilding) {
	for (int32 i = 0; i < buildingsArray.Num(); i++) {
		if (buildingsArray[i] == inBuilding) {
			buildingsArray[i]->KillMe();
			buildingsArray.RemoveAt(i, 1, true);
			counterArray.RemoveAt(i, 1, true);
			break;
		}
	}
}