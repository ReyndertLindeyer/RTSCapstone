// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGunCharacter.h"
#include "UObject/ConstructorHelpers.h" 

// Sets default values
AMyGunCharacter::AMyGunCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	selected = false;
}

// Called when the game starts or when spawned
void AMyGunCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyGunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}