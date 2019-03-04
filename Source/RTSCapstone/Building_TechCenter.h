// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "Building_TechCenter.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_TechCenter : public ABuildingMaster
{
	GENERATED_BODY()
public:
	ABuilding_TechCenter();

	virtual void InitializeStructure(II_Player* player) override;
};
