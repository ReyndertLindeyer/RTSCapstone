// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"

#include "UNIT_Grinder.h"
#include "UNIT_Gattling.h"
#include "UNIT_Roomba.h"
#include "UNIT_Prism.h"

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

	UPROPERTY(EditAnywhere)
		AActor* setPlayerOwner;

	void SpawnUnit();

	void SetupBulding(FVector inVec);

	UPROPERTY(EditAnywhere)
		int32 buildingType;

	virtual void BeginPlay() override;
	
	void SetMesh(UStaticMesh* inMesh);

private:
	FVector waypoint; 

};
