// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuildingMaster.h"
#include "Building_Construction_Yard.h"
#include "Building_PowerPlant.h"
#include "Building_Barrecks.h"
#include "Building_Refinery.h"
#include "Building_VehicleFactory.h"
#include "Building_TechCenter.h"
#include "BuildingManagerObject.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API UBuildingManagerObject : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UBuildingManagerObject();

	void ghostBuilding(uint8 whatBuilding, FVector spawnLocation);

	void MoveBuilding(FVector location);

	ABuildingMaster* GetBuildingToBuild();

	void SpawnConstructionYard(FVector spawnLocation);

	bool constructBuilding();

	bool trainInfantry(uint8 whatInfantry, ABuilding_Barrecks* whatBuilding);

	float GetResources();

	TArray<int32> GetBuildingCost();

	TArray<int32> GetConstructionTime();

	int32 GetCurrentPower();

	int32 GetMaxPower();

	void SubtractCost(int32 whatBuilding);
	void AddCost(int32 whatBuilding);

	void EnableAllDecals();
	void DisableAllDecals();

	void CheckForDestroyedBuildings();

	bool IsTechCentreBuilt();
	bool IsRefineryBuilt();


private:

	UMaterial* canBuildIndicator;
	UMaterial* cantBuildIndicator;
	UMaterial* regularMaterial;

	int32 currentPower, maxPower, resources;

	TArray<int32> buildingCosts;
	TArray<int32> buildingConstructionTimes;

	TArray <ABuilding_PowerPlant*> powerPlantArray;
	TArray <ABuilding_Barrecks*> barrecksArray;
	TArray <ABuilding_Refinery*> refineryArray;
	TArray <ABuilding_VehicleFactory*> vehicleFactoryArray;
	TArray <ABuilding_TechCenter*> techCenterArray;
	ABuilding_Construction_Yard* constructionYard;

	TArray <ABuildingMaster*> masterArray;

	ABuildingMaster* buildingToBuild;
};
