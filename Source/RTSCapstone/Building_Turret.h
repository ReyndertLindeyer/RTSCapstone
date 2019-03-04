// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"

#include "Components/StaticMeshComponent.h"

#include "Building_Turret.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_Turret : public ABuildingMaster
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;

	uint8 range;

	virtual void InitializeStructure(II_Player* player) override;
};
