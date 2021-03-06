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
#include "Building_Superweapon.h"
#include "Building_Turret_Gattling.h"
#include "Building_Turret_Cannon.h"
#include "Building_Turret_Artillery.h"
#include "Building_Turret_Tesla.h"
#include "Building_Outpost.h"

#include "Building_Ghost.h"

#include "Engine/DataTable.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

#include "I_Player.h"

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

	void ghostBuilding(uint8 whatBuilding_, FVector spawnLocation);

	void MultiGhostBuilding(uint8 whatBuilding_, FVector spawnLocation);

	void MoveBuilding(FVector location);

	ABuilding_Ghost* GetBuildingToBuild();

	void SpawnConstructionYard(FVector spawnLocation);

	bool constructBuilding();
	void MultiConstructBuilding();

	void DeleteBuilding();

	TArray<int32> GetBuildingCost();

	TArray<int32> GetConstructionTime();

	int32 GetCertainConstructionTime(int32 whatBuilding);

	int32 GetMaxPower();

	void SubtractCost(int32 whatBuilding_);
	void AddCost(int32 whatBuilding_);

	void EnableAllDecals();
	void DisableAllDecals();

	void CheckForDestroyedBuildings();

	bool IsTechCentreBuilt();
	bool IsRefineryBuilt();

	void SetPlayer(II_Player* inPlayer);

	bool IsConstructionYardDestroyed();

	void ConstructOutpost(FVector spawnLocation);

	UPROPERTY()
		TArray <ABuilding_Ghost*> ghostBuildingArray;
	TArray <int32> whatBuildingArray;

	UPROPERTY()
		TArray <ABuilding_Refinery*> refineryArray;
	UPROPERTY()
		TArray <ABuilding_TechCenter*> techCenterArray;

private:
	UPROPERTY()
		UMaterialInterface* canBuildIndicator;
	UPROPERTY()
		UMaterialInterface* cantBuildIndicator;
	UPROPERTY()
		UMaterialInterface* regularMaterial;

	int32 maxPower, resources, whatBuilding; //What building is currently being built?
	bool power; //Bool for if the player has run out of power, if they have then the manager will check to see if the next building that's being built can provide enough power to boost everything back up

	II_Player* thePlayer;

	TArray<FName> namesArray;

	UPROPERTY()
		TArray<int32> buildingCosts;
	UPROPERTY()
		TArray<int32> buildingConstructionTimes;
	UPROPERTY()
		TArray<int32> buildingPowerConsumption;
	UPROPERTY()
		TArray<int32> buildingMaxHealth;

	
	UPROPERTY()
		ABuilding_Construction_Yard* constructionYard;
	UPROPERTY()
		TArray <ABuildingMaster*> masterArray;
	UPROPERTY()
		TArray <ABuildingMaster*> turretArray;

	UPROPERTY()
		ABuilding_Ghost* buildingToBuild;


	UPROPERTY()
		TArray <UStaticMesh*> buildingMeshArray;
};
