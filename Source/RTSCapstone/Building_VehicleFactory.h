// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
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

	void AddToUnitQueue(int unitType);
	void SpawnUnit(int unitType);
	void SetWaypoint(FVector inVec);
	uint8 GetUnitCost(uint8 whatUnit);

protected:
	virtual void Tick(float DeltaTime) override;

	bool constructingUnit;

	TArray <uint8> unitQueue;

	float countToCompleteUnit;

	int32 harvesterBuildTime, harvesterCost, humveeTime, humveeCost, tankTime, tankCost, artilleryTankBuildTime, artilleryTankCost, heavyTankTime, heavyTankCost, outpostTime, outpostCost;
};
