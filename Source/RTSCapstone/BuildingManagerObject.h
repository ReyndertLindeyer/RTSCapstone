// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuildingMaster.h"
#include "Building_PowerPlant.h"
#include "Building_Barrecks.h"
#include "Building_Refinery.h"
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

	uint32 power, resources;

	TArray <ABuildingMaster*> buildingArray;

	ABuildingMaster* ghostBuilding(uint8 whatBuilding, FVector spawnLocation);

	bool constructBuilding(ABuildingMaster* toBuild);
};
