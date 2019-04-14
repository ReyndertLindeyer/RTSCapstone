// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceNode.h"

// Sets default values
AResourceNode::AResourceNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Tags.Add(FName("ResourceNode"));

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResourceMesh"));

	int32 tempNum = FMath::RandRange(0, 2);

	if (tempNum == 0) {
		mesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/ResourcePiles/ResourceOne.ResourceOne")).Get());
	}
	else if (tempNum == 1) {
		mesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/ResourcePiles/ResourceTwo.ResourceTwo")).Get());
	}
	else {
		mesh->SetStaticMesh(ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(TEXT("/Game/Game_Assets/Models/ResourcePiles/ResourceThree.ResourceThree")).Get());
	}
	mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	RootComponent = mesh;
	//mesh->SetupAttachment(RootComponent);
	mesh->SetCollisionProfileName(FName("OverlapAll"));
	mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mesh->SetSimulatePhysics(false);

	tempNum = FMath::RandRange(0, 5);

	if (tempNum == 0) {
		resourceMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("/Game/Game_Assets/Materials/ResourceMaterials/MAT_Emissive_Blue")).Get();
	}
	else if (tempNum == 1) {
		resourceMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("/Game/Game_Assets/Materials/ResourceMaterials/MAT_Emissive_Green")).Get();
	}
	else if (tempNum == 20) {
		resourceMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("/Game/Game_Assets/Materials/ResourceMaterials/MAT_Emissive_Orange")).Get();
	}
	else if (tempNum == 3) {
		resourceMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("/Game/Game_Assets/Materials/ResourceMaterials/MAT_Emissive_Purple")).Get();
	}
	else if (tempNum == 4) {
		resourceMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("/Game/Game_Assets/Materials/ResourceMaterials/MAT_Emissive_Red")).Get();
	}
	else {
		resourceMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("/Game/Game_Assets/Materials/ResourceMaterials/MAT_Emissive_Yellow")).Get();
	}

	mesh->SetMaterial(0, resourceMaterial);

	needsRespawning = false;
	isOccupied = false;
}

void AResourceNode::setup(UINT32 resourceYield, bool canRespawn)
{
	yield = resourceYield;
	respawnable = canRespawn;
}

float AResourceNode::Harvest()
{
	if (yield - 10 >= 0)
	{
		yield -= 10.0f;
		return 10.0f;
	}

	else
	{
		float amt = yield;
		yield = 0;
		return amt;
	}
}

float AResourceNode::RemainingResources()
{
	return yield;
}

// Called every frame
void AResourceNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (yield <= 0 && respawnable) {
		Destroy(this);
		//SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 20.0f));
		//respawnTime = FMath::RandRange(30.0f, 60.0f);
		//needsRespawning = true;
	}
	if (yield <= 0 && !respawnable) {
		Destroy(this);
	}
	if (respawnTime >= 0.0f && needsRespawning) {
		yield = FMath::RandRange(100, 120);
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 20.0f));
		needsRespawning = false;
	}
	if (needsRespawning) {
		respawnTime -= DeltaTime;
	}
}