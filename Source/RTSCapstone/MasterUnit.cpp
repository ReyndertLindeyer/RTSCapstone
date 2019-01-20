// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterUnit.h"

// Sets default values
AMasterUnit::AMasterUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	dangerDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	//All units start unselected
	selected = false;
}

// Called when the game starts or when spawned
void AMasterUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMasterUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

