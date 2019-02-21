// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "Building_Barrecks.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_Barrecks : public ABuildingMaster
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
		FVector wayPoint;

	ABuilding_Barrecks();

	int32 AddToUnitQueue(int unitType);
	int32 RemoveFromUnitQueue();
	void SpawnUnit(int unitType);
	void SetWaypoint(FVector inVec);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	bool constructingUnit;

	TArray <uint8> unitQueue;

	float countToCompleteUnit;

	uint8 rifleBuildTime, rifleInfantryCost, rocketBuildTime, rocketInfantryCost, engineerBuildTime, engineerCost;
};