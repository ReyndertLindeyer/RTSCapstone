// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "Engine/StaticMesh.h"

//All of the units
#include "UNIT_AvBT.h"
#include "UNIT_MBT.h"
#include "UNIT_MArtillery.h"
#include "UNIT_Harvester.h"

#include "Building_VehicleFactory.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_VehicleFactory : public ABuildingMaster
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		FVector wayPoint;

	ABuilding_VehicleFactory();

	UPROPERTY(EditAnywhere)
		AActor* setPlayerOwner;

	int32 AddToUnitQueue(int32 unitType);
	int32 RemoveFromUnitQueue();
	int32 GetUnitAtStartOfQueue();
	float StartingTime(); //How long the unit takes to complete
	float TimeRemaining(); //How long until the unit is completed
	void SpawnUnit();
	void SetWaypoint(FVector inVec);
	void SetHasPower(bool inBool); //Sets the boolean for if the structure has enough power


protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	bool constructingUnit, hasPower; //Variables for if the building is constructing any units, and if the building has enough power

	TArray <uint8> unitQueue;

	float countToCompleteUnit;

	int32 harvesterBuildTime, harvesterCost, humveeTime, humveeCost, tankTime, tankCost, artilleryTankBuildTime, artilleryTankCost, heavyTankTime, heavyTankCost, outpostTime, outpostCost;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* waypointMesh;
};
