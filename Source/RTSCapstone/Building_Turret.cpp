// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Turret.h"
#include "ConstructorHelpers.h"

void ABuilding_Turret::InitializeStructure(II_Player* player)
{
	InitializeEntity(player, "Turret", 1200.0f);
}

void ABuilding_Turret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UE_LOG(LogTemp, Warning, TEXT("TURRET IS TICKING"));

}