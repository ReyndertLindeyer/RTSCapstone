// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingManagerObject.h"

// Sets default values
UBuildingManagerObject::UBuildingManagerObject()
{
	currentPower = 20;
	maxPower = currentPower;
	resources = 5000; 
	powerPlantCost = 100;
	refineryCost = 300;
	barracksCost = 200;
	powerPlantConstructionTime = 1;
	refineryConstructionTime = 1;
	barracksConstructionTime = 1;
}

ABuildingMaster * UBuildingManagerObject::ghostBuilding(uint8 whatBuilding, FVector spawnLocation)
{
	UWorld* const World = this->GetWorld();
	if (World)
	{
		ABuildingMaster* building;
		if (whatBuilding == 1) {
			building = World->SpawnActor<ABuilding_PowerPlant>(ABuilding_PowerPlant::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));

			if (building) {
				EnableAllDecals();
				return building;
			}
		}
		else if (whatBuilding == 2) {
			building = World->SpawnActor<ABuilding_Refinery>(ABuilding_Refinery::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));

			if (building) {
				EnableAllDecals();
				return building;
			}
		}
		else if (whatBuilding == 3) {
			building = World->SpawnActor<ABuilding_Barrecks>(ABuilding_Barrecks::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f));

			if (building) {
				EnableAllDecals();
				return building;
			}
		}
	}

	return nullptr;
}

ABuildingMaster * UBuildingManagerObject::getBuilding(int32 indexOfBuilding)
{
	return buildingArray[indexOfBuilding];
}

void UBuildingManagerObject::SpawnConstructionYard(FVector spawnLocation)
{
	buildingArray.Add(GetWorld()->SpawnActor<ABuilding_Construction_Yard>(ABuilding_Construction_Yard::StaticClass(), spawnLocation, FRotator(0.0f, 0.0f, 0.0f)));
}

bool UBuildingManagerObject::constructBuilding(ABuildingMaster * toBuild)
{
	if ((int32)toBuild->GetCost() < resources && toBuild->constructAtLocation()) {
		currentPower -= toBuild->GetPowerUsage();
		if (toBuild->GetPowerUsage() < 0) {
			maxPower -= toBuild->GetPowerUsage();
		}
		buildingArray.Add(toBuild); 
		DisableAllDecals();
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


int32 UBuildingManagerObject::GetBuildingCost(uint8 whatBuilding) {
	if (whatBuilding == 1) {
		return (int32)powerPlantCost;
	}
	else if (whatBuilding == 2) {
		return (int32)refineryCost;
	}
	else if (whatBuilding == 3) {
		return (int32)barracksCost;
	}

	return 0;
}

int32 UBuildingManagerObject::GetConstructionTime(uint8 whatBuilding)
{
	if (whatBuilding == 1) {
		return (int32)powerPlantConstructionTime;
	}
	else if (whatBuilding == 2) {
		return (int32)refineryConstructionTime;
	}
	else if (whatBuilding == 3) {
		return (int32)barracksConstructionTime;
	}
	return 0;
}

int32 UBuildingManagerObject::GetCurrentPower()
{
	return currentPower;
}

int32 UBuildingManagerObject::GetMaxPower()
{
	return maxPower;
}

void UBuildingManagerObject::SubtractCost(int32 whatBuilding)
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

void UBuildingManagerObject::AddCost(int32 whatBuilding)
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

void UBuildingManagerObject::EnableAllDecals()
{
	if (buildingArray.Num() > 0) {
		for (int32 i = 0; i < buildingArray.Num(); i++) {
			buildingArray[i]->EnableBuildDecal();
		}
	}
}

void UBuildingManagerObject::DisableAllDecals()
{
	if (buildingArray.Num() > 0) {
		for (int32 i = 0; i < buildingArray.Num(); i++) {
			buildingArray[i]->DisableBuildDecal();
		}
	}
}

void UBuildingManagerObject::CheckForDestroyedBuildings()
{
	if (buildingArray.Num() > 0) {
		for (int i = 0; i < buildingArray.Num(); i++) {
			if (buildingArray[i]->IsDead()) {
				ABuildingMaster * temp = buildingArray[i];
				buildingArray.Remove(temp);
				temp->Suicide();
			}
		}
	}
}
