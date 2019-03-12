// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "Building_Superweapon.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_Superweapon : public ABuildingMaster
{
	GENERATED_BODY()
public:
	ABuilding_Superweapon();

	virtual void Tick(float DeltaTime) override;

	virtual void InitializeStructure(II_Player* player) override;

	void SetHasPower(bool inBool); //Sets the boolean for if the structure has enough power

private:
	bool hasPower;
};
