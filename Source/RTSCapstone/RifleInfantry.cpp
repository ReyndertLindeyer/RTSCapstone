// Fill out your copyright notice in the Description page of Project Settings.

#include "RifleInfantry.h"
#include "MyRTSAIController.h"

ARifleInfantry::ARifleInfantry() {
	maxHealth = 10;
	currentHealth = maxHealth;

	root = GetCapsuleComponent();
	RootComponent = root;
	root->InitCapsuleSize(6.0f, 15.0f);
	root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	root->SetSimulatePhysics(true);


	charMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SoldierMesh"));
	///This can break as hardcoded asset references can break during packaging
	charMesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/Placeholder_Soldier_Gun_Cylinder001.Placeholder_Soldier_Gun_Cylinder001")).Get());
	charMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -8.0f));
	charMesh->SetupAttachment(RootComponent);

	isPlayer = true;

	sightRadius = 80.0f;
	attackRadius = 60.0f;

	//AIControllerClass = CreateDefaultSubobject<AMyRTSAIController>(TEXT("AIController"));
}

// Called when the game starts or when spawned
void ARifleInfantry::BeginPlay()
{
	Super::BeginPlay();
}