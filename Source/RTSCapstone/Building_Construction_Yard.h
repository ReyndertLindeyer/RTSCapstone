// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "Building_Construction_Yard.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_Construction_Yard : public ABuildingMaster
{
	GENERATED_BODY()
	
public:
	ABuilding_Construction_Yard();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void constructAtLocation() override;
};
