// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "Building_PlanetaryCannon.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_PlanetaryCannon : public ABuildingMaster
{
	GENERATED_BODY()

public:
	ABuilding_PlanetaryCannon();

	UPROPERTY(EditAnywhere)
		AActor* setPlayerOwner;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
