// Fill out your copyright notice in the Description page of Project Settings.

#include "Unit_Harvester.h"

AUnit_Harvester::AUnit_Harvester() {
	maxHealth = 200;
	currentHealth = maxHealth;
	travelSpeed = 10;
	spawnTime = 20;

	harvestingSpeed = 5;

	sightRadius = 100;


	unitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HarvesterMesh"));
	unitMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/yeetHarvyDev.yeetHarvyDev")).Get());
	unitMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	RootComponent = unitMesh;
	unitMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	unitMesh->SetCollisionProfileName(TEXT("Trigger"));
	unitMesh->OnComponentBeginOverlap.AddDynamic(this, &AUnit_Harvester::BeginOverlap);
	unitMesh->OnComponentEndOverlap.AddDynamic(this, &AUnit_Harvester::OnOverlapEnd);
	unitMesh->SetSimulatePhysics(false);
}

void AUnit_Harvester::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) 
{
}

void AUnit_Harvester::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}