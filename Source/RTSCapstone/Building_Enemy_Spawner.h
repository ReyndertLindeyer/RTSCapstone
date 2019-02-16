// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Building_Enemy_Spawner.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_Enemy_Spawner : public AActor
{
	GENERATED_BODY()
	
public:
	ABuilding_Enemy_Spawner();

	void SpawnUnit(int unitType);

	void SetBuldingType(FVector inVec, UStaticMesh* inMesh, int32 buildingType);

private:
	FVector waypoint; 
	
	UPROPERTY()
		UStaticMeshComponent* buildingMesh;

	int32 type, maxHealth, currentHealth;
};
