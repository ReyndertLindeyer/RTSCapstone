// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_BaseManager.h"

// Sets default values
AEnemy_BaseManager::AEnemy_BaseManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	isStartingArea = false;
	PrimaryActorTick.bCanEverTick = isStartingArea;
	baseRadius = 10;

	baseRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("baseRadius"));
	baseRadiusSphere->SetSphereRadius(baseRadius);
	baseRadiusSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	baseRadiusSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_BaseManager::BeginOverlap);
	baseRadiusSphere->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemy_BaseManager::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = isStartingArea;
}

// Called every frame
void AEnemy_BaseManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int32 i = 0; i < buildingsArray.Num(); i++) {
		counterArray[i] -= DeltaTime;
		if (counterArray[i] <= 0.0f) {
			buildingsArray[i]->SpawnUnit();
			counterArray[i] = FMath::RandRange(10.0f, 15.0f);
		}
		//if buildingsArray[i] health is zero then destroy it and remove it from the array, if there are none remaining then activate the adjacent managers
	}

}

void AEnemy_BaseManager::ActivateManager() {
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemy_BaseManager::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		if (Cast<ABuilding_Enemy_Spawner>(OtherActor))
		{
			Cast<ABuilding_Enemy_Spawner>(OtherActor)->SetupBulding(launchPoint->GetActorLocation());
			buildingsArray.Add(Cast<ABuilding_Enemy_Spawner>(OtherActor));
			counterArray.Add(FMath::RandRange(10.0f, 15.0f));
		}
	}
}