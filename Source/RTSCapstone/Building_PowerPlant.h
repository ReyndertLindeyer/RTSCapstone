// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingMaster.h"
#include "Building_PowerPlant.generated.h"

/**
 * 
 */
UCLASS()
class RTSCAPSTONE_API ABuilding_PowerPlant : public ABuildingMaster
{
	GENERATED_BODY()
public:
	ABuilding_PowerPlant();

	UPROPERTY(EditAnywhere)
		AActor* setPlayerOwner;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual bool constructAtLocation(II_Player* player) override;

	void Upgrade();

};
