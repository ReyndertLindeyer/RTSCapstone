// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "Building_PowerPlant.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_PowerPlant : public ABuildingMaster
{
	GENERATED_BODY()
public:
	ABuilding_PowerPlant();
	virtual void BeginPlay() override;
	void Upgrade();

	virtual void InitializeStructure(II_Player* player) override;
};
