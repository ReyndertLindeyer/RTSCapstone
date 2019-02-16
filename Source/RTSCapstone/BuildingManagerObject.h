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
	UPROPERTY()
		UMaterial* canBuildIndicator;
	UPROPERTY()
		UMaterial* cantBuildIndicator;
	UPROPERTY()
		UMaterial* regularMaterial;

	int32 currentPower, maxPower, resources;

	UPROPERTY()
		TArray<int32> buildingCosts;
	UPROPERTY()
		TArray<int32> buildingConstructionTimes;

	UPROPERTY()
		TArray <ABuilding_PowerPlant*> powerPlantArray;
	UPROPERTY()
		TArray <ABuilding_Barrecks*> barrecksArray;
	UPROPERTY()
		TArray <ABuilding_Refinery*> refineryArray;
	UPROPERTY()
		TArray <ABuilding_VehicleFactory*> vehicleFactoryArray;
	UPROPERTY()
		TArray <ABuilding_TechCenter*> techCenterArray;
	UPROPERTY()
		ABuilding_Construction_Yard* constructionYard;
	UPROPERTY()
		TArray <ABuildingMaster*> masterArray;

	UPROPERTY()
		ABuildingMaster* buildingToBuild;
};
