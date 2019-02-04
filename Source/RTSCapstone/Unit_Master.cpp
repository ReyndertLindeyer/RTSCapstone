// Fill out your copyright notice in the Description page of Project Settings.

#include "Unit_Master.h"

// Sets default values
AUnit_Master::AUnit_Master()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

float AUnit_Master::GetSightRadius()
{
	return sightRadius;
}

// Called when the game starts or when spawned
void AUnit_Master::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnit_Master::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

