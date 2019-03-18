// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Superweapon.h"

#include "Kismet/KismetSystemLibrary.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

#include "Projectile.h"

ABuilding_Superweapon::ABuilding_Superweapon() {
	PrimaryActorTick.bCanEverTick = true;
	//Setting up general values
	team = 1;
	spawnTime = 2;
	buildRadius = 500;
	isBuilding = true;

	buildingMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Power_Plant.Placeholder_Power_Plant")).Get());
	buildingMesh->SetSimulatePhysics(false);

	decal->SetupAttachment(RootComponent);
	decal->DecalSize = FVector(3, buildRadius, buildRadius);

	buildingMesh->ComponentTags.Add(FName("Building"));
	decal->ComponentTags.Add(FName("BuildArea"));

	PS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_RocketShooting.P_RocketShooting'")).Get();
	reactionPS = ConstructorHelpers::FObjectFinderOptional<UParticleSystem>(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_Explosion.P_Explosion'")).Get();

	// Change to true for debugging
	isReady = true;
	currentTimer = 0.0f;
}

void ABuilding_Superweapon::BeginPlay()
{
	Super::BeginPlay();

	if (setPlayerOwner != nullptr)
		InitializeStructure(Cast<II_Player>(setPlayerOwner), "Superweapon", 1200.0f);

}

void ABuilding_Superweapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("TURRET IS TICKING"));

	if (constructed)
	{

		// Activate the super weapon
		if (isReady)
		{
			
		}

		// Count down the timer for the super weapon
		if (currentTimer > 0)
		{
			currentTimer -= DeltaTime;
		}

		else if (currentTimer <= 0)
		{
			isReady = true;
		}
	}


	

}

void ABuilding_Superweapon::SetHasPower(bool inBool)
{
	//Disable attack functions
}