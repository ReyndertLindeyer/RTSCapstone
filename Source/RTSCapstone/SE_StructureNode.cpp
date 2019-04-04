// Fill out your copyright notice in the Description page of Project Settings.

#include "SE_StructureNode.h"

// Sets default values
ASE_StructureNode::ASE_StructureNode()
{
 	PrimaryActorTick.bCanEverTick = true;

	origin = CreateDefaultSubobject<USceneComponent>(TEXT("Origin"));
	RootComponent = origin;

}

// Called when the game starts or when spawned
void ASE_StructureNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASE_StructureNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

