// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "I_Unit.h"


#include "Building_Refinery.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_Refinery : public ABuildingMaster
{
	GENERATED_BODY()
	
public:
	ABuilding_Refinery();

	UPROPERTY(EditAnywhere)
		USceneComponent* harvestPt;

	

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	virtual void InitializeStructure(II_Player* player) override;

	bool isOccupied;

private:
	bool canSpawnHarvester;
};
