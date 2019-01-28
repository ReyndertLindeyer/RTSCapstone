// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "Building_Turret.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_Turret : public ABuildingMaster
{
	GENERATED_BODY()
	
public:
	uint8 range;
};
