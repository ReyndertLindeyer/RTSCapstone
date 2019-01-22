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

protected:
	virtual void BeginPlay() override;


	UINT32 rifleBuildTime, rifleInfantryCost, rocketBuildTime, rocketInfantryCost, harvesterBuildTime, harvesterCost;
};
