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

	int32 AddToUnitQueue(int32 unitType);
	int32 RemoveFromUnitQueue();
	int32 GetUnitAtStartOfQueue();
	float StartingTime(); //How long the unit takes to complete
	float TimeRemaining(); //How long until the unit is completed
	void SpawnUnit();
	void SetWaypoint(FVector inVec);

	virtual void InitializeStructure(II_Player* player) override;

protected:
	virtual void Tick(float DeltaTime) override;

	bool constructingUnit;

	TArray <uint8> unitQueue;

	float countToCompleteUnit;

	int32 harvesterBuildTime, harvesterCost, humveeTime, humveeCost, tankTime, tankCost, artilleryTankBuildTime, artilleryTankCost, heavyTankTime, heavyTankCost, outpostTime, outpostCost;
};
