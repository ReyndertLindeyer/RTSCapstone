// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "UNIT_Rifleman.h"
#include "UNIT_Rocketeer.h"
#include "UNIT_Engineer.h"
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

	int32 AddToUnitQueue(int32 unitType);
	int32 RemoveFromUnitQueue();
	int32 GetUnitAtStartOfQueue();
	float StartingTime(); //How long the unit takes to complete
	float TimeRemaining(); //How long until the unit is completed
	void SpawnUnit();
	void SetWaypoint(FVector inVec);
	void SetHasPower(bool inBool); //Sets the boolean for if the structure has enough power

	virtual void InitializeStructure(II_Player* player) override;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	bool constructingUnit, hasPower; //Variables for if the building is constructing any units, and if the building has enough power

	TArray <uint8> unitQueue;

	float countToCompleteUnit;

	uint32 rifleBuildTime, rifleInfantryCost, rocketBuildTime, rocketInfantryCost, engineerBuildTime, engineerCost;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* waypointMesh;
};