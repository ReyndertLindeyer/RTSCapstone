// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"


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

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	virtual void InitializeStructure(II_Player* player) override;

private:
	bool canSpawnHarvester;
};
