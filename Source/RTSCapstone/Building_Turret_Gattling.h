// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

#include "Building_Turret_Gattling.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_Turret_Gattling : public ABuildingMaster
{
	GENERATED_BODY()
	
public:
	ABuilding_Turret_Gattling();

	virtual void Tick(float DeltaTime) override;

	void SetHasPower(bool inBool); //Sets the boolean for if the structure has enough power

	virtual void InitializeStructure(II_Player* player) override;

private:
	uint32 range;
	bool hasPower;
};
