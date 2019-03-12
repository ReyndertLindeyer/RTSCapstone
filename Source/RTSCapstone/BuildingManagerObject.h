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

#include "Engine/DataTable.h"
#include "Engine/World.h"

#include "I_Player.h"

#include "BuildingManagerObject.generated.h"

USTRUCT(BlueprintType)
struct FBuildingVariables : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 BuildTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 PowerConsumption;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 MaxHealth;
};

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

	void MoveBuilding(FVector location);

	ABuildingMaster* GetBuildingToBuild();

	void SpawnConstructionYard(FVector spawnLocation);

	bool constructBuilding();

	void DeleteBuilding();

	TArray<int32> GetBuildingCost();

	TArray<int32> GetConstructionTime();

	int32 GetMaxPower();

	void SubtractCost(int32 whatBuilding_);
	void AddCost(int32 whatBuilding_);

	void EnableAllDecals();
	void DisableAllDecals();

	void CheckForDestroyedBuildings();

	bool IsTechCentreBuilt();
	bool IsRefineryBuilt();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDataTable* buildingDataTable;

	void SetPlayer(II_Player* inPlayer);

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
		TArray <ABuilding_Refinery*> refineryArray;
	UPROPERTY()
		TArray <ABuilding_TechCenter*> techCenterArray;
	UPROPERTY()
		ABuilding_Construction_Yard* constructionYard;
	UPROPERTY()
		TArray <ABuildingMaster*> masterArray;
	UPROPERTY()
		TArray <ABuildingMaster*> turretArray;

	UPROPERTY()
		ABuildingMaster* buildingToBuild;
};
