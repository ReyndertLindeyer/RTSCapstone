// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_BaseManager.h"

// Sets default values
AEnemy_BaseManager::AEnemy_BaseManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	baseRadius = 10;

	baseRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("baseRadius"));
	baseRadiusSphere->SetSphereRadius(baseRadius);
	baseRadiusSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_BaseManager::BeginOverlap);
	baseRadiusSphere->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemy_BaseManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy_BaseManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy_BaseManager::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		if (Cast<ABuilding_Enemy_Spawner>(OtherActor))
		{
			buildingsArray.Add(Cast<ABuilding_Enemy_Spawner>(OtherActor));
		}
	}
}