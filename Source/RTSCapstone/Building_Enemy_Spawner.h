// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Building_Enemy_Spawner.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_Enemy_Spawner : public ABuildingMaster
{
	GENERATED_BODY()
	
public:
	ABuilding_Enemy_Spawner();

	void SpawnUnit();

	void SetupBulding(FVector inVec, UStaticMesh* inMesh, int32 type);

	UPROPERTY(EditAnywhere)
		int32 buildingType;

private:
	FVector waypoint; 

};
