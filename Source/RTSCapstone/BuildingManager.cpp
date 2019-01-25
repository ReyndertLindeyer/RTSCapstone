// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingManager.h"

// Sets default values
ABuildingManager::ABuildingManager()
{
	power = 20;
	resources = 0;
}

ABuildingMaster * ABuildingManager::ghostBuilding(uint8 whatBuilding, FVector spawnLocation)
{
	ABuildingMaster* building;
	if (whatBuilding == 1) {
		building = GetWorld()->SpawnActor<ABuilding_PowerPlant>(ABuilding_PowerPlant::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		return building;
	}
	else if (whatBuilding == 2) {
		building = GetWorld()->SpawnActor<ABuilding_Refinery>(ABuilding_Refinery::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		return building;
	}
	else if (whatBuilding == 3) {
		building = GetWorld()->SpawnActor<ABuilding_Barrecks>(ABuilding_Barrecks::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		return building;
	}

	return nullptr;
}

bool ABuildingManager::constructBuilding(ABuildingMaster * toBuild)
{
	if (toBuild->GetCost() > resources) {
		resources -= toBuild->GetCost();
		power -= toBuild->GetPowerUsage();
		buildingArray.Add(toBuild);
		return true;
	}
	return false;
}
