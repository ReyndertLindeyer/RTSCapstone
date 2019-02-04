// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingManagerObject.h"

// Sets default values
UBuildingManagerObject::UBuildingManagerObject()
{
	power = 20;
	resources = 5000; 
	powerPlantCost = 100;
	refineryCost = 300;
	barracksCost = 200;
	powerPlantConstructionTime = 10;
	refineryConstructionTime = 20;
	barracksConstructionTime = 15;
}

ABuildingMaster * UBuildingManagerObject::ghostBuilding(uint8 whatBuilding, FVector spawnLocation)
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

bool UBuildingManagerObject::constructBuilding(ABuildingMaster * toBuild)
{
	if (toBuild->GetCost() < resources && toBuild->constructAtLocation()) {
		power -= toBuild->GetPowerUsage();
		buildingArray.Add(toBuild);
		return true;
	}
	return false;
}

bool UBuildingManagerObject::trainInfantry(uint8 whatInfantry, ABuilding_Barrecks * whatBuilding)
{
	if (whatBuilding->GetUnitCost(whatInfantry) <= resources) {
		resources -= whatBuilding->GetUnitCost(whatInfantry);
		whatBuilding->AddToUnitQueue(whatInfantry);
		return true;
	}
	return false;
}

float UBuildingManagerObject::GetResources() {
	return (float)resources;
}


int UBuildingManagerObject::GetBuildingCost(uint8 whatBuilding) {
	if (whatBuilding == 1) {
		return (int)powerPlantCost;
	}
	else if (whatBuilding == 2) {
		return (int)refineryCost;
	}
	else if (whatBuilding == 3) {
		return (int)barracksCost;
	}

	return 0;
}

int UBuildingManagerObject::GetConstructionTime(uint8 whatBuilding)
{
	if (whatBuilding == 1) {
		return (int)powerPlantConstructionTime;
	}
	else if (whatBuilding == 2) {
		return (int)refineryConstructionTime;
	}
	else if (whatBuilding == 3) {
		return (int)barracksConstructionTime;
	}
	return 0;
}

void UBuildingManagerObject::SubtractCost(int whatBuilding)
{
	if (whatBuilding == 1) {
		resources -= powerPlantCost;
	}
	else if (whatBuilding == 2) {
		resources -= refineryCost;
	}
	else if (whatBuilding == 3) {
		resources -= barracksCost;
	}
}

void UBuildingManagerObject::AddCost(int whatBuilding)
{
	if (whatBuilding == 1) {
		resources += powerPlantCost;
	}
	else if (whatBuilding == 2) {
		resources += refineryCost;
	}
	else if (whatBuilding == 3) {
		resources += barracksCost;
	}
}
